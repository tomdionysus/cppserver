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
#include "tcp_session.h"

#include <functional>
#include <iostream>

#include "logger_scoped.h"

namespace cppserver {

TCPSession::TCPSession(std::shared_ptr<Logger> logger, std::shared_ptr<boost::asio::ip::tcp::socket> connection)
    : _connection(connection),
      _logger(std::make_unique<LoggerScoped>(connection->remote_endpoint().address().to_string() + ":" + std::to_string(connection->remote_endpoint().port()),
                                             logger)),
      _running(false),
      _thread(std::make_unique<std::thread>(std::bind(&TCPSession::_execute, this, 0))),
      _rx_timer(_rx_wait_context, boost::asio::chrono::seconds(10)) {}

TCPSession::~TCPSession() { close(); }

void TCPSession::close() {
  if (_running) {
    // Signal Thread
    _running = false;

    // Cancel reading
    _rx_timer.cancel();
    _connection->close();
  }

  // Wait for thread quit
  if (_thread->joinable()) {
    _thread->join();
  }
}

void TCPSession::_execute(int id) {
  _running = true;

  _logger->info("Connected");

  char buffer[65535];

  while (_running) {
    boost::system::error_code ec;

    ssize_t len = _read_with_timeout(buffer, 65535, 5000, ec);

    if (ec) {
      if (ec == boost::asio::error::operation_aborted) {
        // Timed out, normal operation
      } else if (ec == boost::asio::error::eof) {
        _logger->info("Remote Closed Connection");
        _running = false;
      } else {
        _logger->error("Closing (Error during read: " + ec.what() + ")");
        _running = false;
      }
    } else {
      _logger->debug("Read " + std::to_string(len) + " bytes");
    }
  }

  if (_connection->is_open()) {
    _connection->close();
  }

  _logger->info("Closed");
}

ssize_t TCPSession::_read_with_timeout(void *ptr, size_t len, uint32_t timeout_ms, boost::system::error_code &ec) {
  if (!ptr || len == 0) return -1;  // Validate input parameters

  ssize_t rec_len = 0;

  // Set the timeout
  _rx_timer.expires_after(boost::asio::chrono::milliseconds(timeout_ms));

  // Do the async read
  _connection->async_read_some(boost::asio::buffer(ptr, len), [&](const boost::system::error_code &error, std::size_t length) {
    if (!error) {
      rec_len = length;
    } else {
      rec_len = -1;
    }
    ec = error;
    _rx_timer.cancel();
  });

  // Set up the asynchronous wait on the _rx_timer
  _rx_timer.async_wait([&](const boost::system::error_code &error) {
    if (error) {
      if (error == boost::asio::error::operation_aborted) {
        // Normal operation.
      }

    } else {
      _connection->cancel();
      ec = boost::asio::error::operation_aborted;
    }
  });

  // Wait for the timeout, or read to complete
  _rx_wait_context.run();
  _rx_wait_context.reset();

  return static_cast<ssize_t>(rec_len);  // Successfully read 'length' bytes
}

}  // namespace cppserver