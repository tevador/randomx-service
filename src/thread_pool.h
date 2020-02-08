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

#pragma once

#include "task_queue.h"
#include <vector>
#include <list>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace randomx {

	class Service;
	class ServiceWorker;

	class ThreadPool : public httplib::TaskQueue<ServiceWorker> {
	public:
		ThreadPool(Service& server, size_t n);

		ThreadPool(const ThreadPool&) = delete;
		virtual ~ThreadPool();

		virtual void enqueue(std::function<void(ServiceWorker&)> fn) override;

		void reseed(ServiceWorker& self, const void* seed, size_t length);

		virtual void shutdown() override;

		const Service& getService() {
			return svc_;
		}

	private:
		friend struct ServiceWorker;

		Service& svc_;
		std::vector<std::shared_ptr<std::thread>> threads_;
		std::vector<std::shared_ptr<ServiceWorker>> workers_;
		std::list<std::function<void(ServiceWorker&)>> jobs_;

		bool shutdown_;
		bool reseeding_;

		std::condition_variable cond_;
		std::mutex mutex_;
	};

}