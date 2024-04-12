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

#include <boost/program_options.hpp>
#include <cstdint>
#include <string>

#include "logger.h"
#include "url.h"

namespace cppserver {

enum DBMode { FILE, MYSQL, POSTGRESQL };

class Config {
 public:
  DBMode dbMode;
  URL dbUrl;
  uint16_t port;

  Config(std::shared_ptr<Logger> logger);
  Config(std::shared_ptr<Logger> logger, int argc, char* argv[]);

  void parse_cmd_line(int argc, char* argv[]);
  bool is_valid();

 private:
  std::unique_ptr<Logger> _logger;

  bool _valid;

  static const std::map<std::string, uint16_t> defaultPorts;
};

}  // namespace cppserver
