//
// cppserverd
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
#include <boost/asio/signal_set.hpp>
#include <iostream>
#include <optional>
#include <string>

#include "config.h"
#include "device_db_mysql.h"
#include "logger_scoped.h"
#include "logger_stdio.h"
#include "tcp_server.h"
#include "url.h"
#include "util.h"
#include "version.h"

using namespace std;
using namespace cppserver;

int main(int argc, char* argv[]) {
  auto mainLogger = std::make_shared<LoggerStdIO>(LogLevel::DEBUG);

  // This is v0.1.0
  Version version(0, 1, 0);

  mainLogger->info("-----------------------------------");
  mainLogger->info("cppserverd v" + version.to_string());
  mainLogger->info("-----------------------------------");

  Config config(mainLogger, argc, argv);

  if (!config.is_valid()) {
    mainLogger->error("Invalid Configuration");
    return 99;
  }

  DeviceDBMySQL ddMysql(mainLogger, config.dbUrl);
  DeviceDB& deviceDb = ddMysql;

  // Create the device database
  deviceDb.initialise();

  // Create the TcpServer instance with the logger and start it on the specified port
  TCPServer tcpServer(mainLogger, 26547);
  Server& server = tcpServer;

  // Wait for SIGINT
  boost::asio::io_context signal_wait_context;
  boost::asio::signal_set signals(signal_wait_context, SIGINT);

  signals.async_wait([&](const boost::system::error_code& error, int signal_number) {
    if (!error) {
      mainLogger->debug("SIGINT received");
      server.stop();
    }
  });

  // Start TCP server
  server.start();

  // Start other server processes...

  // Do the SIGINT Wait
  signal_wait_context.run();
}
