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
#pragma once

#include <cstdint>
#include <mutex>
#include <vector>

namespace cppserver {

class Random {
 public:
  Random();
  ~Random();

  void initialize();
  int8_t get_int8();
  int16_t get_int16();
  int32_t get_int32();
  int64_t get_int64();
  void get_random(void* ptr, size_t size);

 private:
  std::vector<char> buffer;
  size_t buffer_index;
  std::mutex mtx;

  void reset_buffer();
  void refill_buffer(size_t size, void* ptr);
};

}  // namespace cppserver
