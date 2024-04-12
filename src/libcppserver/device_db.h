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

#include <iostream>
#include <string>
#include <vector>

namespace cppserver {

#define AES_KEY_SIZE 32

typedef uint8_t aes_key_t[AES_KEY_SIZE];

class Host {
 public:
  uint64_t id;
  std::string name;
  aes_key_t aes_key;
  std::vector<uint64_t> devices;
};

class Device {
 public:
  uint64_t id;
  std::string name;
  std::string filename;
  uint32_t block_size;
  uint64_t block_total;
  bool read_only;
};

class DeviceDB {
 public:
  virtual bool initialise() = 0;
  virtual std::shared_ptr<Host> get_host(uint64_t host_id) = 0;
  virtual std::shared_ptr<Device> get_device(uint64_t device_id) = 0;
  virtual std::vector<Device> get_host_devices(uint64_t host_id) = 0;
  virtual bool close() = 0;
};

}  // namespace cppserver