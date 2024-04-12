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
#include "url.h"

#include <limits>
#include <string>

namespace cppserver {

URL::URL() = default;

URL::URL(const std::string& url) { parse(url); }

void URL::parse(const std::string& url) {
  // USe boost regex to parse the URL
  boost::regex pattern(R"(^([a-zA-Z][a-zA-Z0-9+.-]*):\/\/(?:([^:\/?#]*):?([^@\/?#]*)@)?([^:\/?#]+)(?::(\d+))?([^?#]*)(?:\?([^#]*))?(?:#(.*))?$)");
  boost::smatch matches;

  // Default is invalid
  _valid = false;

  // If the URL matches
  if (boost::regex_match(url, matches, pattern)) {
    try {
      scheme = matches[1];

      // User/Password is optional
      user = matches[2].matched ? std::optional<std::string>{matches[2]} : std::nullopt;
      password = matches[3].matched ? std::optional<std::string>{matches[3]} : std::nullopt;
      host = matches[4];

      // Port is also optional
      if (matches[5].length() == 0) {
        // If it isn't explicitly set, attempt to infer it from the scheme
        auto it = defaultPorts.find(scheme);
        if (it != defaultPorts.end()) {
          port = it->second;
        } else {
          port = std::nullopt;
        }
      } else {
        // If it is explicitly set check range
        int _port = std::stoi(matches[5]);
        if (_port < 0 || _port > std::numeric_limits<uint16_t>::max()) {
          throw std::out_of_range("The value is out of the range for uint16_t.");
        }
        port = static_cast<uint16_t>(_port);
      }

      if (matches[6].matched) {
        path = matches[6];
      } else {
        path = "/";  // Set default path as "/"
      }

      query = matches[7];
      fragment = matches[8];

      // If we reaach here the URL is valid.
      _valid = true;
    } catch (const std::exception& e) {
      // Something went wrong. Assume invalid.
    }
  }
}

std::string URL::to_string() const {
  std::string url = scheme + "://";
  if (user.has_value()) {
    url += user.value();
    if (password.has_value()) {
      url += ":" + password.value();
    }
    url += "@";
  }
  url += host;
  // Only add the port if it's not the default port for the scheme
  if (port.has_value()) {
    // Convert the scheme to lowercase to match the keys in defaultPorts map
    std::string schemeLower = scheme;
    std::transform(schemeLower.begin(), schemeLower.end(), schemeLower.begin(), [](unsigned char c) { return std::tolower(c); });

    auto it = defaultPorts.find(schemeLower);
    if ((it != defaultPorts.end()) && (it->second != port.value())) {
      // Add port only if it's not the default port for the scheme
      url += ":" + std::to_string(port.value());
    }
  }
  url += path;
  if (!query.empty()) {
    url += "?" + query;
  }
  if (!fragment.empty()) {
    url += "#" + fragment;
  }
  return url;
}

bool URL::is_valid() { return _valid; }

bool operator==(const URL& lhs, const URL& rhs) {
  return lhs.scheme == rhs.scheme && lhs.user == rhs.user && lhs.password == rhs.password && lhs.host == rhs.host && lhs.port == rhs.port &&
         lhs.path == rhs.path && lhs.query == rhs.query && lhs.fragment == rhs.fragment;
}

std::string operator+(const URL& url, const std::string& str) { return url.to_string() + str; }

std::string operator+(const std::string& str, const URL& url) { return str + url.to_string(); }

const std::map<std::string, uint16_t> URL::defaultPorts = {{"http", 80},         {"https", 443},       {"ftp", 21},
                                                           {"mysql", 3306},      {"postgresql", 5432}, {"ssh", 22},
                                                           {"telnet", 23},       {"smtp", 25},         {"dns", 53},
                                                           {"http-alt", 8080},   // Alternative HTTP port
                                                           {"https-alt", 8443},  // Alternative HTTPS port
                                                           {"pop3", 110},        {"imap", 143},        {"ldap", 389},
                                                           {"sftp", 22},                          // SFTP shares port with SSH
                                                           {"snmp", 161},        {"smtps", 465},  // SMTP over SSL
                                                           {"imaps", 993},                        // IMAP over SSL
                                                           {"pop3s", 995},                        // POP3 over SSL
                                                           {"redis", 6379},      {"mongodb", 27017},   {"cassandra", 9042},
                                                           {"memcached", 11211}, {"rabbitmq", 5672},   {"mqtt", 1883},
                                                           {"coap", 5683},       {"amqp", 5671},  // AMQP over TLS
                                                           {"rsync", 873},       {"rdp", 3389},        {"elasticsearch", 9200},
                                                           {"kibana", 5601},     {"zookeeper", 2181}};

}  // namespace cppserver
