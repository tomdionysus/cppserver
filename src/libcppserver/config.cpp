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

#include "config.h"

#include "logger_scoped.h"

namespace po = boost::program_options;

namespace cppserver {

Config::Config(std::shared_ptr<Logger> logger) : _logger(std::make_unique<LoggerScoped>("config", logger)) {}

Config::Config(std::shared_ptr<Logger> logger, int argc, char* argv[]) : _logger(std::make_unique<LoggerScoped>("config", logger)) {
  parse_cmd_line(argc, argv);
}

void Config::parse_cmd_line(int argc, char* argv[]) {
  _logger->debug("Parsing command line...");
  _valid = true;

  try {
    po::options_description desc("Allowed options");

    // clang-format off
    desc.add_options()
      ("help,h", "Help")
      ("db_mode", po::value<std::string>(), "Device Database Mode (file, mysql, postgresql)")
      ("db_url", po::value<std::string>(), "A Database URL (mysql://user:password@host/database, etc)");
    // clang-format on

    po::variables_map vm;

    // Create a command line parser that applies the 'desc' options
    po::command_line_parser parser(argc, argv);
    parser.options(desc).allow_unregistered();  // Allow unknown options
    po::parsed_options parsed_options = parser.run();

    // Store and notify parsed options
    po::store(parsed_options, vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return;
    }

    // Process options
    if (vm.count("db_url")) {
      dbUrl.parse(vm["db_url"].as<std::string>());

      if (dbUrl.is_valid()) {
        _logger->debug("db_url.scheme = " + dbUrl.scheme);
        if (dbUrl.user) {
          _logger->debug("db_url.user = " + dbUrl.user.value());
        }
        if (dbUrl.password) {
          _logger->debug("db_url.password = " + dbUrl.password.value());
        }
        _logger->debug("db_url.host = " + dbUrl.host);
        if (dbUrl.port) {
          _logger->debug("db_url.port = " + std::to_string(dbUrl.port.value()));
        }
        _logger->debug("db_url.path = " + dbUrl.path);
        _logger->debug("db_url.query = " + dbUrl.query);
        _logger->debug("db_url.fragment = " + dbUrl.fragment);
      } else {
        _logger->warn("db_url is not a valid URL");
        _valid = false;
      }
    }

    // Collect all unrecognized options from the parsed information
    std::vector<std::string> unrecognized_opts = po::collect_unrecognized(parsed_options.options, po::include_positional);

    // Handle or display unrecognized options
    if (!unrecognized_opts.empty()) {
      for (const auto& opt : unrecognized_opts) {
        _logger->error("Unknown: " + opt);
      }
      _valid = false;
    }

  } catch (const po::error& e) {
    _logger->error("Error: " + std::string(e.what()));
    _valid = false;
  }
}

bool Config::is_valid() { return _valid; }

}  // namespace cppserver