#include <gtest/gtest.h>

#include <string>

#include "url.h"

namespace cppserver {

class URLTest : public ::testing::Test {
 protected:
  // Helper function to test URL parsing and validation
  void testURLParsing(const std::string& input_url, bool expected_validity, const std::string& expected_scheme, const std::optional<std::string>& expected_user,
                      const std::optional<std::string>& expected_password, const std::string& expected_host, const std::optional<uint16_t>& expected_port,
                      const std::string& expected_path, const std::string& expected_query, const std::string& expected_fragment) {
    URL url(input_url);
    EXPECT_EQ(url.is_valid(), expected_validity);
    if (expected_validity) {
      EXPECT_EQ(url.scheme, expected_scheme);
      EXPECT_EQ(url.user, expected_user);
      EXPECT_EQ(url.password, expected_password);
      EXPECT_EQ(url.host, expected_host);
      EXPECT_EQ(url.port, expected_port);
      EXPECT_EQ(url.path, expected_path);
      EXPECT_EQ(url.query, expected_query);
      EXPECT_EQ(url.fragment, expected_fragment);
    }
  }
};

// Test correct parsing of a full URL
TEST_F(URLTest, ParseFullURL) {
  testURLParsing("https://user:pass@host:8080/path?query#fragment", true, "https", "user", "pass", "host", 8080, "/path", "query", "fragment");
}

// Test URL with missing parts and default behaviors
TEST_F(URLTest, ParseURLMissingParts) { testURLParsing("http://host", true, "http", std::nullopt, std::nullopt, "host", 80, "", "", ""); }

// Test invalid port number
TEST_F(URLTest, InvalidPort) {
  URL url("http://host:99999");
  EXPECT_FALSE(url.is_valid());
}

// Test URL serialization
TEST_F(URLTest, Serialization) {
  URL url("https://user:pass@host:8080/path?query#fragment");
  std::string serialized_url = url.to_string();
  EXPECT_EQ(serialized_url, "https://user:pass@host:8080/path?query#fragment");
}

// Test adding path to URL using operator+
TEST_F(URLTest, ConcatenatePath) {
  URL url("https://host");
  std::string full_url = url + "/newpath";
  EXPECT_EQ(full_url, "https://host/newpath");
}

// Test invalid URL
TEST_F(URLTest, InvalidURL) {
  URL url("ht@tp://:");
  EXPECT_FALSE(url.is_valid());
}

}  // namespace cppserver
