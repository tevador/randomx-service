/*
Copyright (c) 2020, tevador <tevador@gmail.com>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holder nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "service_worker.h"
#include "thread_pool.h"
#include "service.h"

namespace randomx {

	ServiceWorker::ServiceWorker(ThreadPool& pool, unsigned id) :
		pool_(pool), 
		vm_(pool.getService().createMachine()),
		idle_(true),
		id_(id)
	{
	}

	ServiceWorker::~ServiceWorker() {
		pool_.getService().destroyMachine(vm_);
	}

	void ServiceWorker::operator()() {
		for (;;) {
			{
				std::unique_lock<std::mutex> lock(mutex_);
				idle_ = true;
				cond_.notify_one();
			}
			std::function<void(ServiceWorker&)> fn;
			{
				std::unique_lock<std::mutex> lock(pool_.mutex_);
				pool_.cond_.wait(
					lock, [&] { return (!pool_.reseeding_ && !pool_.jobs_.empty()) || pool_.shutdown_; });

				if (pool_.shutdown_ && pool_.jobs_.empty()) { break; }

				fn = pool_.jobs_.front();
				pool_.jobs_.pop_front();
				idle_ = false;
			}
			fn(*this);
		}
	}

	void ServiceWorker::waitIdle() {
		std::unique_lock<std::mutex> lock(mutex_);
		cond_.wait(
			lock, [&] { return idle_; });
	}
}