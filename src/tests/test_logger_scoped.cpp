#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "logger.h"
#include "logger_scoped.h"

using ::testing::_;
using ::testing::StrEq;

namespace cppserver {

class MockLogger : public Logger {
 public:
  MOCK_METHOD(void, debug, (const std::string &log), (override));
  MOCK_METHOD(void, info, (const std::string &log), (override));
  MOCK_METHOD(void, warn, (const std::string &log), (override));
  MOCK_METHOD(void, error, (const std::string &log), (override));
};

class LoggerScopedTest : public ::testing::Test {
 protected:
  std::shared_ptr<MockLogger> mock_logger;
  std::unique_ptr<LoggerScoped> logger_scoped;

  void SetUp() override {
    mock_logger = std::make_shared<MockLogger>();
    logger_scoped = std::make_unique<LoggerScoped>("TestScope", mock_logger);
  }

  void TearDown() override {
    mock_logger.reset();
    logger_scoped.reset();
  }
};

TEST_F(LoggerScopedTest, DebugLogsCorrectMessage) {
  EXPECT_CALL(*mock_logger, debug(StrEq("(TestScope) message"))).Times(1);
  logger_scoped->debug("message");
}

TEST_F(LoggerScopedTest, InfoLogsCorrectMessage) {
  EXPECT_CALL(*mock_logger, info(StrEq("(TestScope) message"))).Times(1);
  logger_scoped->info("message");
}

TEST_F(LoggerScopedTest, WarnLogsCorrectMessage) {
  EXPECT_CALL(*mock_logger, warn(StrEq("(TestScope) message"))).Times(1);
  logger_scoped->warn("message");
}

TEST_F(LoggerScopedTest, ErrorLogsCorrectMessage) {
  EXPECT_CALL(*mock_logger, error(StrEq("(TestScope) message"))).Times(1);
  logger_scoped->error("message");
}

}  // namespace cppserver
