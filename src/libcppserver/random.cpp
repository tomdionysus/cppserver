//
// cppserver
//
// Copyright (C) 2024 Tom Cully
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301, USA.
//
#include "random.h"

#include <cstring>  // For memcpy
#include <fstream>
#include <iostream>
#include <mutex>

namespace cppserver {

Random::Random() : buffer(1024 * 1024, 0), buffer_index(0) {
  std::lock_guard<std::mutex> lock(mtx);
  initialize();  // Automatically initialize on construction
}

Random::~Random() {}

void Random::initialize() { reset_buffer(); }

void Random::reset_buffer() {
  refill_buffer(buffer.size(), buffer.data());
  buffer_index = 0;
}

void Random::get_random(void* ptr, size_t size) {
  std::lock_guard<std::mutex> lock(mtx);
  if (size <= buffer.size() - buffer_index) {
    // There is enough data left in the buffer, use it
    memcpy(ptr, &buffer[buffer_index], size);
    buffer_index += size;
  } else {
    // Not enough data left in the buffer, read directly from /dev/urandom
    refill_buffer(size, ptr);
  }
}

void Random::refill_buffer(size_t size, void* ptr) {
  std::ifstream randFile("/dev/urandom", std::ios::in | std::ios::binary);
  if (!randFile) {
    std::cerr << "Cannot open /dev/urandom" << std::endl;
    throw std::runtime_error("Failed to open /dev/urandom");
  }

  // Read directly into the buffer or provided pointer
  randFile.read(static_cast<char*>(ptr), size);
  if (!randFile) {
    std::cerr << "Error reading from /dev/urandom" << std::endl;
    throw std::runtime_error("Failed to read from /dev/urandom");
  }
  randFile.close();
}

int8_t Random::get_int8() {
  std::lock_guard<std::mutex> lock(mtx);
  if (buffer_index + sizeof(int8_t) > buffer.size()) reset_buffer();
  int8_t result = *reinterpret_cast<int8_t*>(&buffer[buffer_index]);
  buffer_index += sizeof(int8_t);
  return result;
}

int16_t Random::get_int16() {
  std::lock_guard<std::mutex> lock(mtx);
  if (buffer_index + sizeof(int16_t) > buffer.size()) reset_buffer();
  int16_t result = *reinterpret_cast<int16_t*>(&buffer[buffer_index]);
  buffer_index += sizeof(int16_t);
  return result;
}

int32_t Random::get_int32() {
  std::lock_guard<std::mutex> lock(mtx);
  if (buffer_index + sizeof(int32_t) > buffer.size()) reset_buffer();
  int32_t result = *reinterpret_cast<int32_t*>(&buffer[buffer_index]);
  buffer_index += sizeof(int32_t);
  return result;
}

int64_t Random::get_int64() {
  std::lock_guard<std::mutex> lock(mtx);
  if (buffer_index + sizeof(int64_t) > buffer.size()) reset_buffer();
  int64_t result = *reinterpret_cast<int64_t*>(&buffer[buffer_index]);
  buffer_index += sizeof(int64_t);
  return result;
}

}  // namespace cppserver
