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

#include <iostream>
#include <stdexcept>
#include "utility.h"
#include "service.h"

void printUsage(const char* exe) {
	std::cout << "RandomX Service v" RANDOMX_SERVICE_VERSION << std::endl;
	std::cout << "Usage: " << exe << " [OPTIONS]" << std::endl;
	std::cout << "Supported options:" << std::endl;
	std::cout << "  -host <string>     Bind to a specific address (default: localhost)" << std::endl
		<< "  -port <number>     Bind to a specific port (default: 39093)" << std::endl
		<< "  -threads <number>  Use a specific number of threads (default: all CPU threads)" << std::endl
		<< "  -flags <number>    Use specific RandomX flags (default: auto)" << std::endl
		<< "  -origin <string>   Allow cross-origin requests from a specific web page" << std::endl
		<< "  -log               Log all HTTP requests to stdout" << std::endl
		<< "  -help              Display this message" << std::endl;
}

int main(int argc, char** argv) {
	std::string host, origin;
	int port, threads, flags;
	bool help, log;

	readStringOption("-host", argc, argv, host, "localhost");
	readIntOption("-port", argc, argv, port, 39093);
	readIntOption("-threads", argc, argv, threads, randomx::Service::getMachineThreads());
	readIntOption("-flags", argc, argv, flags, randomx::Service::getAutoFlags());
	readStringOption("-origin", argc, argv, origin, "");
	readOption("-log", argc, argv, log);
	readOption("-help", argc, argv, help);

	if (help) {
		printUsage(argv[0]);
		return 0;
	}

	try {
		std::cout << "Initializing service..." << std::endl;
		randomx::Service svc(threads, flags);
		std::cout << "Threads: " << threads << ", Flags: " << svc.getFlags() << std::endl;
		if (!origin.empty()) {
			std::cout << "Setting origin to " << origin << std::endl;
			svc.setOrigin(origin);
		}
		if (log) {
			std::cout << "Logging is enabled" << std::endl;
			svc.enableLog();
		}
		std::cout << "Binding to " << host << ":" << port << "..." << std::endl;
		if (!svc.run(host.data(), port)) {
			throw std::runtime_error("Failed to bind");
		}
	}
	catch (const std::exception & e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}