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

#include <boost/regex.hpp>
#include <cstdint>
#include <iostream>
#include <optional>
#include <string>

namespace cppserver {

class URL {
 public:
  std::string scheme;
  std::optional<std::string> user;
  std::optional<std::string> password;
  std::string host;
  std::optional<uint16_t> port;
  std::string path;
  std::string query;
  std::string fragment;

  URL();
  URL(const std::string& url);

  void parse(const std::string& url);
  std::string to_string() const;
  bool is_valid();

  friend bool operator==(const URL& lhs, const URL& rhs);

  friend std::string operator+(const URL& url, const std::string& str);
  friend std::string operator+(const std::string& str, const URL& url);

 private:
  bool _valid{false};

  bool isDefaultPort() const;
  std::string toLower(const std::string& input) const;

  static const std::map<std::string, uint16_t> defaultPorts;
};

}  // namespace cppserver
