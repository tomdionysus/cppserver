#include <gtest/gtest.h>

#include "util.h"

namespace cppserver {

class UtilTest : public ::testing::Test {
 protected:
  UtilTest() {
    // Setup code, if needed
  }

  ~UtilTest() override {
    // Cleanup code, if needed
  }
};

// Test to_hex using std::vector<uint8_t>
TEST_F(UtilTest, VectorToHex_EmptyVector) {
  std::vector<uint8_t> empty_vec;
  EXPECT_EQ(Util::to_hex(empty_vec), "");
}

TEST_F(UtilTest, VectorToHex_SingleElement) {
  std::vector<uint8_t> vec = {0xAB};
  EXPECT_EQ(Util::to_hex(vec), "ab");
}

TEST_F(UtilTest, VectorToHex_MultipleElements) {
  std::vector<uint8_t> vec = {0xAB, 0xCD, 0xEF};
  EXPECT_EQ(Util::to_hex(vec), "abcdef");
}

TEST_F(UtilTest, VectorToHex_Zeroes) {
  std::vector<uint8_t> vec = {0x00, 0x00, 0x00};
  EXPECT_EQ(Util::to_hex(vec), "000000");
}

// Test to_hex using uint8_t array and length
TEST_F(UtilTest, ArrayToHex_EmptyArray) {
  uint8_t arr[] = {};
  EXPECT_EQ(Util::to_hex(arr, 0), "");
}

// Adjust the test code to check for correct output more robustly
TEST_F(UtilTest, ArrayToHex_SingleElement) {
  uint8_t arr[] = {0xAB};
  auto output = Util::to_hex(arr, 1);
  for (auto &c : output) c = std::tolower(c);  // Ensure lowercase comparison
  EXPECT_EQ(output, "ab");
}

TEST_F(UtilTest, ArrayToHex_MultipleElements) {
  uint8_t arr[] = {0xAB, 0xCD, 0xEF};
  auto output = Util::to_hex(arr, 3);
  for (auto &c : output) c = std::tolower(c);  // Ensure lowercase comparison
  EXPECT_EQ(output, "abcdef");
}

TEST_F(UtilTest, ArrayToHex_Zeroes) {
  uint8_t arr[] = {0x00, 0x00, 0x00};
  auto output = Util::to_hex(arr, 3);
  for (auto &c : output) c = std::tolower(c);  // Ensure lowercase comparison
  EXPECT_EQ(output, "000000");
}

}  // namespace cppserver
