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
#include "device_db_mysql.h"

#include <mysql/mysql.h>

#include <cstring>
#include <iostream>
#include <string>

#include "logger_scoped.h"
#include "url.h"

namespace cppserver {

DeviceDBMySQL::DeviceDBMySQL(std::shared_ptr<Logger> logger, URL& dbUrl) : _dbUrl(dbUrl), _logger(std::make_unique<LoggerScoped>("mysql", logger)) {}

DeviceDBMySQL::~DeviceDBMySQL() { close(); }

bool DeviceDBMySQL::initialise() {
  if (!_dbUrl.is_valid()) {
    _logger->error("dbUrl is not valid.");
    return false;
  }

  conn = mysql_init(nullptr);

  if (!conn) {
    _logger->error("MySQL initialization failed.");
    return false;
  }

  const char* user = nullptr;
  const char* password = nullptr;
  const char* database = _dbUrl.path.length() > 1 ? _dbUrl.path.substr(1).c_str() : nullptr;
  uint16_t port = 0;

  if (_dbUrl.user) user = _dbUrl.user.value().c_str();
  if (_dbUrl.password) password = _dbUrl.password.value().c_str();
  if (_dbUrl.port) port = _dbUrl.port.value();

  _logger->debug("connecting...");

  if (!mysql_real_connect(conn, _dbUrl.host.c_str(), user, password, database, port, nullptr, 0)) {
    _logger->error("Connection failed: " + std::string(mysql_error(conn)));
    return false;
  }

  _logger->info("Connected");

  return true;
}

std::shared_ptr<Host> DeviceDBMySQL::get_host(uint64_t host_id) {
  // Do Query
  if (mysql_query(conn, ("SELECT id, name, aes_key FROM host WHERE id = " + std::to_string(host_id)).c_str())) {
    _logger->error("Query failed: " + std::string(mysql_error(conn)));
    return NULL;
  }

  // Get Result
  MYSQL_RES* result = mysql_store_result(conn);
  if (!result) {
    _logger->error("Query host: Bad Result Set: " + std::string(mysql_error(conn)));
    return NULL;
  }

  // Check Fields
  int num_fields = mysql_num_fields(result);

  if (num_fields != 3) {
    _logger->error("Query host: Expected 3 fields, got " + std::to_string(num_fields));
    mysql_free_result(result);
    return NULL;
  }

  std::shared_ptr<Host> host = std::make_shared<Host>();

  // Get The Data
  MYSQL_ROW row = mysql_fetch_row(result);
  host->id = std::stoll(std::string(row[0]));
  host->name = std::string(row[1]);

  if (row[2]) {  // Make sure the field is not NULL
    unsigned long* lengths = mysql_fetch_lengths(result);
    if (lengths[2] == 32) {  // Ensure the binary data is exactly 32 bytes
      std::memcpy(host->aes_key, row[2], 32);
    } else {
      _logger->error("AES key length mismatch. Expected 32 bytes.");
      mysql_free_result(result);
      return NULL;
    }
  } else {
    _logger->error("AES key is NULL.");
    mysql_free_result(result);
    return NULL;
  }

  mysql_free_result(result);
  return host;
}

std::shared_ptr<Device> DeviceDBMySQL::get_device(uint64_t device_id) { return NULL; }

std::vector<Device> DeviceDBMySQL::get_host_devices(uint64_t host_id) { return std::vector<Device>(); }

bool DeviceDBMySQL::close() {
  _logger->debug("closing...");
  if (conn) {
    mysql_close(conn);
    conn = nullptr;
  }
  _logger->info("Closed");
  return true;
}

}  // namespace cppserver
