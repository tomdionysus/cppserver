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
#include <util.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

namespace cppserver {

std::string Util::to_hex(const std::vector<uint8_t>& vec) {
  std::ostringstream oss;
  oss << std::hex << std::setfill('0');
  for (const auto& num : vec) {
    oss << std::setw(2) << static_cast<unsigned int>(num);
  }
  return oss.str();
}

std::string Util::to_hex(const uint8_t arr[], uint16_t len) {
  std::ostringstream oss;
  oss << std::hex << std::setfill('0');
  for (uint16_t i = 0; i < len; i++) {
    // Cast to unsigned int to ensure numeric interpretation
    oss << std::setw(2) << static_cast<uint>(arr[i]);
  }
  return oss.str();
}

}  // namespace cppserver