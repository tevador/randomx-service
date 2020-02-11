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

#include <string>
#include <memory>

#define RANDOMX_SERVICE_VERSION "1.0.2"

namespace httplib {
	struct Request;
	struct Response;
}

struct randomx_vm;

namespace randomx {

	struct ServiceWorker;
	struct ServicePrivate;

	class Service {
	public:
		Service(size_t, int);
		~Service();
		bool run(const char* hostname, int port);
		randomx_vm* createMachine() const;
		void destroyMachine(randomx_vm* machine) const;
		void refreshMachine(randomx_vm* machine) const;
		void reinitCache(const void* seed, size_t seedSize);
		void reinitDataset();
		bool checkSeed(const httplib::Request& req);
		void setOrigin(const std::string& origin);
		void enableLog();
		bool allowCors(const char* method, const httplib::Request& req, httplib::Response& res);
		static int getAutoFlags();
		static int getMachineThreads();
		int getFlags() const;
	private:
		std::unique_ptr<ServicePrivate> data_;
	};

}
