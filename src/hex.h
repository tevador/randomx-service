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
#include <climits>

constexpr char hexmap[] = "0123456789abcdef";

inline std::string bin2hex(const char* data, size_t size) {
	std::string hex;
	hex.resize(size * 2);
	for (unsigned i = 0; i < size; ++i) {
		hex[2 * i + 0] = hexmap[(data[i] & 0xF0) >> 4];
		hex[2 * i + 1] = hexmap[data[i] & 0x0F];
	}
	return hex;
}

inline static char parseNibble(uint8_t hex) {
	if (hex > 0x40 && hex <= 0x46) {
		return hex - ('A' - 10);
	}
	if (hex > 0x60 && hex <= 0x66) {
		return hex - ('a' - 10);
	}
	else if (hex >= 0x30 && hex <= 0x39) {
		return hex & 0xF;
	}
	return CHAR_MAX;
}

inline bool hex2bin(const char* in, int length, char* out) {
	if ((length % 2) != 0) {
		return false;
	}
	for (int i = 0; i < length; i += 2) {
		char nibble1 = parseNibble(*in++);
		if (nibble1 == CHAR_MAX)
			return false;
		char nibble2 = parseNibble(*in++);
		if (nibble2 == CHAR_MAX)
			return false;
		*out++ = nibble1 << 4 | nibble2;
	}
	return true;
}
