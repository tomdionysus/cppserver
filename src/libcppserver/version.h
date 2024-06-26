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
#include <sstream>
#include <string>

namespace cppserver {

class Version {
 public:
  Version(uint8_t* ptr, size_t len);
  Version(const std::string& version);
  Version(uint8_t pmajor, uint8_t pminor, uint8_t ppatch);

  size_t pack(uint8_t* ptr);
  std::string to_string();

  friend std::ostream& operator<<(std::ostream& os, const Version& v);

  uint8_t major;
  uint8_t minor;
  uint8_t patch;
};

}  // namespace cppserver