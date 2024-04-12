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
#include "version.h"

#include <sstream>
#include <string>

namespace cppserver {

Version::Version(uint8_t* ptr, size_t len) : major(0), minor(0), patch(0) {
  if (len > 0) major = ptr[0];
  if (len > 1) minor = ptr[1];
  if (len > 2) patch = ptr[2];
}

Version::Version(const std::string& versionString) : major(0), minor(0), patch(0) {
  std::istringstream iss(versionString);
  std::string token;
  int numTokens = 0;

  while (std::getline(iss, token, '.') && numTokens < 3) {
    int num = std::stoi(token);
    if (num < 0 || num > 255) {
      throw std::runtime_error("Version number out of range (0-255)");
    }
    switch (numTokens) {
      case 0:
        major = num;
        break;
      case 1:
        minor = num;
        break;
      case 2:
        patch = num;
        break;
    }
    ++numTokens;
  }
}

Version::Version(uint8_t pmajor, uint8_t pminor, uint8_t ppatch) {
  major = pmajor;
  minor = pminor;
  patch = ppatch;
}

size_t Version::pack(uint8_t* ptr) {
  ptr[0] = major;
  ptr[1] = minor;
  ptr[2] = patch;
  return 3;
}

std::string Version::to_string() {
  std::ostringstream versionString;
  versionString << (uint32_t)major << "." << (uint32_t)minor << "." << (uint32_t)patch;
  return versionString.str();
}

std::ostream& operator<<(std::ostream& os, const Version& v) {
  os << (uint32_t)v.major << "." << (uint32_t)v.minor << "." << (uint32_t)v.patch;
  return os;
}

}  // namespace cppserver