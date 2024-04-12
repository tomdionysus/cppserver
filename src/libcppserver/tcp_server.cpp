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
#include "tcp_server.h"

#include <iostream>

#include "logger_scoped.h"
#include "tcp_session.h"

namespace cppserver {

TCPServer::TCPServer(std::shared_ptr<Logger> logger, short port)
    : _port(port),
      _acceptor(_io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      _logger(std::make_shared<LoggerScoped>("server", logger)) {
  start_accept();
}

TCPServer::~TCPServer() { stop(); }

void TCPServer::start() {
  _logger->debug("Starting...");
  _thread = std::make_shared<std::thread>([this]() { _io_context.run(); });
  _logger->info("Listening on TCP " + std::to_string(_port));
}

void TCPServer::stop() {
  if (_thread) {
    _logger->debug("Stopping...");
    _io_context.stop();

    if (_thread->joinable()) {
      _thread->join();
      _thread.reset();
    }
    _logger->info("Stopped");
  }
}

void TCPServer::start_accept() {
  auto new_connection = std::make_shared<boost::asio::ip::tcp::socket>(_io_context);

  _acceptor.async_accept(*new_connection, boost::bind(&TCPServer::_handle_accept, this, boost::asio::placeholders::error, new_connection));
}

void TCPServer::_handle_accept(const boost::system::error_code& error, std::shared_ptr<boost::asio::ip::tcp::socket> new_connection) {
  if (!error) {
    // Add the new connection to the map
    int id = next_connection_id_++;

    _connections[id] = std::make_shared<TCPSession>(_logger, new_connection);

    _logger->info("New Connection #" + std::to_string(id) + " (" + new_connection->remote_endpoint().address().to_string() + ")");

    // Start accepting another connection
    start_accept();
  } else {
    _logger->error("Error accepting new connection");
  }
}

}  // namespace cppserver
