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

#include <mysql/mysql.h>

#include <iostream>
#include <string>
#include <vector>

#include "device_db.h"
#include "logger.h"
#include "url.h"

namespace cppserver {

class DeviceDBMySQL : public DeviceDB {
 public:
  DeviceDBMySQL(std::shared_ptr<Logger> logger, URL& dbUrl);
  ~DeviceDBMySQL();

  virtual bool initialise();
  virtual std::shared_ptr<Host> get_host(uint64_t host_id);
  virtual std::shared_ptr<Device> get_device(uint64_t device_id);
  virtual std::vector<Device> get_host_devices(uint64_t host_id);
  virtual bool close();

 private:
  std::unique_ptr<Logger> _logger;

  URL _dbUrl;
  struct MYSQL* conn = nullptr;
};

}  // namespace cppserver