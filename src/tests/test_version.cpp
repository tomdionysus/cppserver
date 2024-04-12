#include <gtest/gtest.h>
#include "version.h"
#include <sstream>
#include <stdexcept>

namespace cppserver {

class VersionTest : public ::testing::Test {};

// Test string-based constructor for valid input
TEST_F(VersionTest, StringConstructorValid) {
    Version v("1.23.45");
    EXPECT_EQ(v.major, 1);
    EXPECT_EQ(v.minor, 23);
    EXPECT_EQ(v.patch, 45);
}

// Test string-based constructor for invalid input
TEST_F(VersionTest, StringConstructorInvalid) {
    EXPECT_THROW({
        Version v("256.1.1");
    }, std::runtime_error);
}

// Test array-based constructor for valid input
TEST_F(VersionTest, ArrayConstructorValid) {
    uint8_t data[3] = {12, 34, 56};
    Version v(data, sizeof(data));
    EXPECT_EQ(v.major, 12);
    EXPECT_EQ(v.minor, 34);
    EXPECT_EQ(v.patch, 56);
}

// Test array-based constructor for short array
TEST_F(VersionTest, ArrayConstructorShortArray) {
    uint8_t data[2] = {12, 34};
    Version v(data, sizeof(data));
    EXPECT_EQ(v.major, 12);
    EXPECT_EQ(v.minor, 34);
    EXPECT_EQ(v.patch, 0);  // Expect the default value
}

// Test the pack method
TEST_F(VersionTest, PackMethod) {
    Version v(1, 2, 3);
    uint8_t buffer[3];
    size_t packedSize = v.pack(buffer);
    EXPECT_EQ(packedSize, 3);
    EXPECT_EQ(buffer[0], 1);
    EXPECT_EQ(buffer[1], 2);
    EXPECT_EQ(buffer[2], 3);
}

// Test to_string method
TEST_F(VersionTest, ToStringMethod) {
    Version v(1, 2, 3);
    std::string versionStr = v.to_string();
    EXPECT_EQ(versionStr, "1.2.3");
}

// Test stream output operator
TEST_F(VersionTest, StreamOutputOperator) {
    Version v(1, 2, 3);
    std::ostringstream os;
    os << v;
    EXPECT_EQ(os.str(), "1.2.3");
}

}  // namespace cppserver
