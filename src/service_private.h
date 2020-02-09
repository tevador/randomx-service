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

#include <cstdint>
#include <atomic>
#include <iostream>
#include <climits>
#include <string>
#include "../RandomX/src/randomx.h"
#include "httplib.h"
#include "thread_pool.h"

namespace randomx {

	class Service;
	class ServiceWorker;

	struct ServicePrivate {
		static const int AutoFlags = INT_MAX;
		ServicePrivate(Service& svc, int threads, int flags)
			:
			server_([&svc, threads] { return new ThreadPool(svc, threads); }),
			cache_(nullptr),
			dataset_(nullptr),
			threads_(threads),
			initialized_(false)
		{
			bool autoFlags = flags == AutoFlags;
			if (autoFlags) {
				flags = randomx_get_flags() | RANDOMX_FLAG_FULL_MEM | RANDOMX_FLAG_LARGE_PAGES;
			}
			cache_ = randomx_alloc_cache((randomx_flags)flags);
			if (autoFlags && cache_ == nullptr) {
				std::cout << "RANDOMX_FLAG_LARGE_PAGES was not successful (randomx_cache)" << std::endl;
				flags &= ~RANDOMX_FLAG_LARGE_PAGES;
				cache_ = randomx_alloc_cache((randomx_flags)flags);
			}
			if (cache_ == nullptr) {
				throw std::runtime_error("randomx_alloc_cache failed");
			}
			if (flags & RANDOMX_FLAG_FULL_MEM) {
				dataset_ = randomx_alloc_dataset((randomx_flags)flags);
				if (dataset_ == nullptr) {
					if (autoFlags) {
						std::cout << "RANDOMX_FLAG_LARGE_PAGES was not successful (randomx_dataset)" << std::endl;
						flags &= ~RANDOMX_FLAG_LARGE_PAGES;
						dataset_ = randomx_alloc_dataset((randomx_flags)flags);
						if (dataset_ == nullptr) {
							std::cout << "RANDOMX_FLAG_FULL_MEM was not successful" << std::endl;
							flags &= ~RANDOMX_FLAG_FULL_MEM;
						}
					}
					else {
						throw std::runtime_error("randomx_alloc_dataset failed");
					}
				}
			}
			flags_ = (randomx_flags)flags;
		}

		~ServicePrivate() {
			if (cache_ != nullptr) {
				randomx_release_cache(cache_);
			}
			if (dataset_ != nullptr) {
				randomx_release_dataset(dataset_);
			}
		}

		randomx_dataset* dataset_;
		randomx_cache* cache_;
		httplib::Server<ServiceWorker> server_;
		randomx_flags flags_;
		size_t threads_;
		std::string seedHex_;
		std::string origin_;
		bool initialized_;
		std::atomic<uint64_t> hashes_;
	};

}