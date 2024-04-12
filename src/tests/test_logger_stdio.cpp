#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

#include "logger_stdio.h"

namespace cppserver {

class LoggerStdIOTest : public ::testing::Test {
 protected:
  std::streambuf* originalCoutStreamBuf;
  std::ostringstream capturedCout;

  void SetUp() override {
    // Redirect std::cout to capture outputs
    originalCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(capturedCout.rdbuf());
  }

  void TearDown() override {
    // Restore std::cout to its original state
    std::cout.rdbuf(originalCoutStreamBuf);
  }

  std::string getCapturedOutput() { return capturedCout.str(); }

  void clearCapturedOutput() {
    capturedCout.str("");
    capturedCout.clear();
  }
};

// Test debug level logging
TEST_F(LoggerStdIOTest, DebugLog) {
  LoggerStdIO logger(LogLevel::DEBUG);
  logger.debug("Test debug message");
  std::string output = getCapturedOutput();
  ASSERT_TRUE(output.find("[DEBUG] Test debug message") != std::string::npos);
}

// Test info level logging
TEST_F(LoggerStdIOTest, InfoLog) {
  LoggerStdIO logger(LogLevel::INFO);
  logger.info("Test info message");
  std::string output = getCapturedOutput();
  ASSERT_TRUE(output.find("[INFO ] Test info message") != std::string::npos);
}

// Test warn level logging
TEST_F(LoggerStdIOTest, WarnLog) {
  LoggerStdIO logger(LogLevel::WARN);
  logger.warn("Test warn message");
  std::string output = getCapturedOutput();
  ASSERT_TRUE(output.find("[WARN ] Test warn message") != std::string::npos);
}

// Test error level logging
TEST_F(LoggerStdIOTest, ErrorLog) {
  LoggerStdIO logger(LogLevel::ERROR);
  logger.error("Test error message");
  std::string output = getCapturedOutput();
  ASSERT_TRUE(output.find("[ERROR] Test error message") != std::string::npos);
}

// Test log level suppression
TEST_F(LoggerStdIOTest, LogLevelSuppression) {
  LoggerStdIO logger(LogLevel::WARN);
  logger.debug("Should not appear");
  logger.info("Should not appear");
  std::string output = getCapturedOutput();
  ASSERT_TRUE(output.find("Should not appear") == std::string::npos);
}

}  // namespace cppserver
