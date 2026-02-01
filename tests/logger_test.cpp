#include <gtest/gtest.h>
#include "loggers/logger.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <atomic>
#include <cstdlib>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

static std::vector<std::string> ReadLines(const fs::path& p) {
    std::ifstream f(p.string());
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(f, line)) {
        lines.push_back(line);
    }
    return lines;
}

static size_t CountFilesWithExtension(const fs::path& dir, const std::string& ext) {
    size_t n = 0;
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ext) {
            ++n;
        }
    }
    return n;
}

class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_dir_ = fs::temp_directory_path() / ("logger_test_" + std::to_string(s_counter_++));
        fs::create_directories(test_dir_);
        log_path_ = test_dir_ / "test.log";
    }

    void TearDown() override {
        if (fs::exists(test_dir_)) {
            fs::remove_all(test_dir_);
        }
    }

    fs::path test_dir_;
    fs::path log_path_;

private:
    static std::atomic<unsigned> s_counter_;
};

std::atomic<unsigned> LoggerTest::s_counter_{0};

// ============================================================================
// Group 1: Constructor and initialization (5 tests)
// ============================================================================

TEST_F(LoggerTest, ConstructorAndInit_ExplicitPathCreatesFile) {
    Logger logger(log_path_);
    ASSERT_TRUE(fs::exists(log_path_));
}

TEST_F(LoggerTest, ConstructorAndInit_ClearsAndWritesOneLine) {
    Logger logger(log_path_);
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 1u);
}

TEST_F(LoggerTest, ConstructorAndInit_FirstLineIsValidTimestamp) {
    Logger logger(log_path_);
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 1u);
    long long t = std::atoll(lines[0].c_str());
    ASSERT_GT(t, 0);
}

TEST_F(LoggerTest, ConstructorAndInit_RecreateOverwritesLog) {
    {
        Logger logger(log_path_);
        logger.Log("first", 0);
    }
    {
        Logger logger(log_path_);
    }
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 1u);
}

TEST_F(LoggerTest, ConstructorAndInit_CreatesParentDirectory) {
    fs::path deep_path = test_dir_ / "subdir" / "nested" / "file.log";
    Logger logger(deep_path);
    ASSERT_TRUE(fs::exists(deep_path));
    ASSERT_TRUE(fs::is_directory(deep_path.parent_path()));
}

// ============================================================================
// Group 2: Log() — basic logging (5 tests)
// ============================================================================

TEST_F(LoggerTest, LogBasic_OneLogAppearsInFile) {
    Logger logger(log_path_);
    logger.Log("msg");
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 2u);
    ASSERT_EQ(lines[1], "msg 0");
}

TEST_F(LoggerTest, LogBasic_SeveralLogsInOrder) {
    Logger logger(log_path_);
    logger.Log("a");
    logger.Log("b");
    logger.Log("c");
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 4u);
    ASSERT_EQ(lines[1], "a 0");
    ASSERT_EQ(lines[2], "b 0");
    ASSERT_EQ(lines[3], "c 0");
}

TEST_F(LoggerTest, LogBasic_EmptyMessageAllowed) {
    Logger logger(log_path_);
    logger.Log("");
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 2u);
    ASSERT_EQ(lines[1], " 0");
}

TEST_F(LoggerTest, LogBasic_MessageWithSpaces) {
    Logger logger(log_path_);
    logger.Log("a b c");
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 2u);
    ASSERT_EQ(lines[1], "a b c 0");
}

TEST_F(LoggerTest, LogBasic_ExplicitLevelInFile) {
    Logger logger(log_path_);
    logger.Log("x", 5);
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 2u);
    ASSERT_TRUE(lines[1].size() >= 2);
    ASSERT_EQ(lines[1].substr(lines[1].size() - 2), " 5");
}

// ============================================================================
// Group 3: Log() — levels and format (5 tests)
// ============================================================================

TEST_F(LoggerTest, LogLevel_DefaultLevelZero) {
    Logger logger(log_path_);
    logger.Log("a");
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 2u);
    ASSERT_EQ(lines[1], "a 0");
}

TEST_F(LoggerTest, LogLevel_DifferentLevelsWritten) {
    Logger logger(log_path_);
    logger.Log("l1", 1);
    logger.Log("l2", 2);
    logger.Log("l3", 100);
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 4u);
    ASSERT_EQ(lines[1], "l1 1");
    ASSERT_EQ(lines[2], "l2 2");
    ASSERT_EQ(lines[3], "l3 100");
}

TEST_F(LoggerTest, LogLevel_FormatMessageLevel) {
    Logger logger(log_path_);
    logger.Log("hello", 42);
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 2u);
    ASSERT_EQ(lines[1], "hello 42");
}

TEST_F(LoggerTest, LogLevel_ExplicitLevelZero) {
    Logger logger(log_path_);
    logger.Log("msg", 0);
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 2u);
    ASSERT_EQ(lines[1], "msg 0");
}

TEST_F(LoggerTest, LogLevel_LargeLevel) {
    Logger logger(log_path_);
    logger.Log("m", 99999);
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 2u);
    ASSERT_EQ(lines[1], "m 99999");
}

// ============================================================================
// Group 4: Log file format (5 tests)
// ============================================================================

TEST_F(LoggerTest, LogFormat_StructureTimestampThenLogs) {
    Logger logger(log_path_);
    logger.Log("x", 0);
    logger.Log("y", 1);
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 3u);
    long long t = std::atoll(lines[0].c_str());
    ASSERT_GT(t, 0);
    ASSERT_EQ(lines[1], "x 0");
    ASSERT_EQ(lines[2], "y 1");
}

TEST_F(LoggerTest, LogFormat_EachLineEndsWithNewline) {
    Logger logger(log_path_);
    logger.Log("a");
    std::ifstream f(log_path_.string());
    std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    f.close();
    ASSERT_FALSE(content.empty());
    ASSERT_EQ(content.back(), '\n');
}

TEST_F(LoggerTest, LogFormat_OrderOfLogsPreserved) {
    Logger logger(log_path_);
    logger.Log("first", 1);
    logger.Log("second", 2);
    logger.Log("third", 3);
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 4u);
    ASSERT_EQ(lines[1], "first 1");
    ASSERT_EQ(lines[2], "second 2");
    ASSERT_EQ(lines[3], "third 3");
}

TEST_F(LoggerTest, LogFormat_LongMessageSingleLine) {
    std::string long_msg(500, 'w');
    Logger logger(log_path_);
    logger.Log(long_msg, 0);
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 2u);
    ASSERT_EQ(lines[1], long_msg + " 0");
}

TEST_F(LoggerTest, LogFormat_SpecialCharsInMessage) {
    Logger logger(log_path_);
    logger.Log("a1-b2_c3");
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 2u);
    ASSERT_EQ(lines[1], "a1-b2_c3 0");
}

// ============================================================================
// Group 5: CleanLogs — basic logic (5 tests)
// ============================================================================

TEST_F(LoggerTest, CleanLogsBasic_AtMostMaxFilesRemain) {
    fs::path p1 = test_dir_ / "a.log";
    fs::path p2 = test_dir_ / "b.log";
    fs::path p3 = test_dir_ / "c.log";
    {
        Logger l1(p1);
        l1.Log("a", 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        Logger l2(p2);
        l2.Log("b", 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        Logger l3(p3);
        l3.Log("c", 0);
    }
    {
        Logger logger(p1);
        logger.CleanLogs(2, 0);
    }
    ASSERT_EQ(CountFilesWithExtension(test_dir_, ".log"), 2u);
}

TEST_F(LoggerTest, CleanLogsBasic_OldestRemoved) {
    fs::path p1 = test_dir_ / "old.log";
    fs::path p2 = test_dir_ / "new.log";
    {
        Logger l1(p1);
        l1.Log("old", 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        Logger l2(p2);
        l2.Log("new", 0);
    }
    {
        Logger logger(p2);
        logger.CleanLogs(1, 0);
    }
    ASSERT_FALSE(fs::exists(p1));
    ASSERT_TRUE(fs::exists(p2));
}

TEST_F(LoggerTest, CleanLogsBasic_FileWithHighLevelNotRemoved) {
    fs::path p_low = test_dir_ / "low.log";
    fs::path p_high = test_dir_ / "high.log";
    {
        Logger l_low(p_low);
        l_low.Log("low", 0);
        Logger l_high(p_high);
        l_high.Log("high", 5);
    }
    {
        Logger logger(p_low);
        logger.CleanLogs(1, 0);
    }
    ASSERT_TRUE(fs::exists(p_high));
}

TEST_F(LoggerTest, CleanLogsBasic_OnlyLowLevelFilesCountTowardMax) {
    fs::path p1 = test_dir_ / "1.log";
    fs::path p2 = test_dir_ / "2.log";
    {
        Logger l1(p1);
        l1.Log("a", 0);
        Logger l2(p2);
        l2.Log("b", 0);
    }
    {
        Logger logger(p1);
        logger.CleanLogs(2, 0);
    }
    ASSERT_EQ(CountFilesWithExtension(test_dir_, ".log"), 2u);
}

TEST_F(LoggerTest, CleanLogsBasic_LargeMaxFilesRemovesNothing) {
    fs::path p1 = test_dir_ / "1.log";
    fs::path p2 = test_dir_ / "2.log";
    fs::path p3 = test_dir_ / "3.log";
    {
        Logger l1(p1);
        l1.Log("a", 0);
        Logger l2(p2);
        l2.Log("b", 0);
        Logger l3(p3);
        l3.Log("c", 0);
    }
    {
        Logger logger(p1);
        logger.CleanLogs(10, 0);
    }
    ASSERT_EQ(CountFilesWithExtension(test_dir_, ".log"), 3u);
}

// ============================================================================
// Group 6: CleanLogs — edge cases (5 tests)
// ============================================================================

TEST_F(LoggerTest, CleanLogsEdge_ExactlyMaxFilesNoneRemoved) {
    fs::path p1 = test_dir_ / "1.log";
    fs::path p2 = test_dir_ / "2.log";
    fs::path p3 = test_dir_ / "3.log";
    fs::path p4 = test_dir_ / "4.log";
    fs::path p5 = test_dir_ / "5.log";
    {
        Logger l1(p1);
        l1.Log("a", 0);
        Logger l2(p2);
        l2.Log("b", 0);
        Logger l3(p3);
        l3.Log("c", 0);
        Logger l4(p4);
        l4.Log("d", 0);
        Logger l5(p5);
        l5.Log("e", 0);
    }
    {
        Logger logger(p1);
        logger.CleanLogs(5, 0);
    }
    ASSERT_EQ(CountFilesWithExtension(test_dir_, ".log"), 5u);
}

TEST_F(LoggerTest, CleanLogsEdge_OneFileRemains) {
    Logger logger(log_path_);
    logger.Log("only", 0);
    logger.CleanLogs(10, 0);
    ASSERT_TRUE(fs::exists(log_path_));
}

TEST_F(LoggerTest, CleanLogsEdge_LevelCleanBoundaryFileWithLevelOne) {
    Logger logger(log_path_);
    logger.Log("msg", 1);
    logger.CleanLogs(1, 1);
    ASSERT_TRUE(fs::exists(log_path_));
}

TEST_F(LoggerTest, CleanLogsEdge_MaxFilesZeroDeletesAllCandidates) {
    fs::path p1 = test_dir_ / "1.log";
    fs::path p2 = test_dir_ / "2.log";
    {
        Logger l1(p1);
        l1.Log("a", 0);
        Logger l2(p2);
        l2.Log("b", 0);
    }
    {
        Logger logger(p1);
        logger.CleanLogs(0, 0);
    }
    ASSERT_EQ(CountFilesWithExtension(test_dir_, ".log"), 0u);
}

TEST_F(LoggerTest, CleanLogsEdge_SingleLogFileRemains) {
    Logger logger(log_path_);
    logger.Log("x", 0);
    logger.CleanLogs(1, 0);
    ASSERT_TRUE(fs::exists(log_path_));
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 2u);
}

// ============================================================================
// Group 7: CleanLogs — damaged and foreign files (5 tests)
// ============================================================================

TEST_F(LoggerTest, CleanLogsDamaged_NonLogFileRemoved) {
    Logger logger(log_path_);
    logger.Log("main", 0);
    fs::path txt_path = test_dir_ / "other.txt";
    {
        std::ofstream f(txt_path.string());
        f << "not a log\n";
    }
    ASSERT_TRUE(fs::exists(txt_path));
    logger.CleanLogs(10, 0);
    ASSERT_FALSE(fs::exists(txt_path));
}

TEST_F(LoggerTest, CleanLogsDamaged_LogWithOnlyTimestampCanBeRemoved) {
    fs::path empty_log = test_dir_ / "empty.log";
    fs::path full_log = test_dir_ / "full.log";
    {
        Logger l1(empty_log);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    {
        Logger l2(full_log);
        l2.Log("x", 0);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    {
        Logger l3(test_dir_ / "another.log");
        l3.Log("y", 0);
    }
    {
        Logger logger(full_log);
        logger.CleanLogs(2, 0);
    }
    ASSERT_EQ(CountFilesWithExtension(test_dir_, ".log"), 2u);
}

TEST_F(LoggerTest, CleanLogsDamaged_InvalidFirstLineRemoved) {
    fs::path bad_log = test_dir_ / "bad.log";
    {
        std::ofstream f(bad_log.string());
        f << "0\nmsg 1\n";
    }
    {
        Logger logger(log_path_);
        logger.Log("ok", 0);
        logger.CleanLogs(10, 0);
    }
    ASSERT_FALSE(fs::exists(bad_log));
}

TEST_F(LoggerTest, CleanLogsDamaged_LineWithoutValidLevelRemoved) {
    fs::path bad_log = test_dir_ / "bad.log";
    {
        std::ofstream f(bad_log.string());
        f << "1\nno number here\n";
    }
    {
        Logger logger(log_path_);
        logger.Log("ok", 0);
        logger.CleanLogs(10, 0);
    }
    ASSERT_FALSE(fs::exists(bad_log));
}

TEST_F(LoggerTest, CleanLogsDamaged_ValidLogWithLevelZeroNotRemovedByQuota) {
    Logger logger(log_path_);
    logger.Log("msg", 0);
    logger.CleanLogs(1, 0);
    ASSERT_TRUE(fs::exists(log_path_));
}

// ============================================================================
// Group 8: Isolation and integration (5 tests)
// ============================================================================

TEST_F(LoggerTest, Isolation_WorksInTempDirectory) {
    Logger logger(log_path_);
    logger.Log("test", 0);
    ASSERT_TRUE(fs::exists(log_path_));
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 2u);
    ASSERT_EQ(lines[1], "test 0");
}

TEST_F(LoggerTest, Isolation_TwoLoggersTwoFiles) {
    fs::path p1 = test_dir_ / "first.log";
    fs::path p2 = test_dir_ / "second.log";
    {
        Logger l1(p1);
        l1.Log("first", 0);
        Logger l2(p2);
        l2.Log("second", 0);
    }
    ASSERT_EQ(CountFilesWithExtension(test_dir_, ".log"), 2u);
    auto lines1 = ReadLines(p1);
    auto lines2 = ReadLines(p2);
    ASSERT_EQ(lines1.size(), 2u);
    ASSERT_EQ(lines2.size(), 2u);
    ASSERT_EQ(lines1[1], "first 0");
    ASSERT_EQ(lines2[1], "second 0");
}

TEST_F(LoggerTest, Isolation_CleanLogsDoesNotTouchOtherDirectory) {
    fs::path other_dir = fs::temp_directory_path() / ("logger_test_other_" + std::to_string(
        std::chrono::steady_clock::now().time_since_epoch().count()));
    fs::create_directories(other_dir);
    fs::path other_log = other_dir / "other.log";
    {
        Logger logger(log_path_);
        logger.Log("here", 0);
        Logger other_logger(other_log);
        other_logger.Log("there", 0);
    }
    {
        Logger logger(log_path_);
        logger.CleanLogs(0, 0);
    }
    ASSERT_TRUE(fs::exists(other_log));
    if (fs::exists(other_dir)) {
        fs::remove_all(other_dir);
    }
}

TEST_F(LoggerTest, Isolation_LogThenCleanLogsPreservesFile) {
    Logger logger(log_path_);
    logger.Log("a", 0);
    logger.Log("b", 0);
    logger.CleanLogs(10, 0);
    ASSERT_TRUE(fs::exists(log_path_));
    auto lines = ReadLines(log_path_);
    ASSERT_EQ(lines.size(), 3u);
    ASSERT_EQ(lines[1], "a 0");
    ASSERT_EQ(lines[2], "b 0");
}

TEST_F(LoggerTest, Isolation_MultipleCleanLogsCallsStable) {
    fs::path p1 = test_dir_ / "1.log";
    fs::path p2 = test_dir_ / "2.log";
    fs::path p3 = test_dir_ / "3.log";
    {
        Logger l1(p1);
        l1.Log("a", 0);
        Logger l2(p2);
        l2.Log("b", 0);
        Logger l3(p3);
        l3.Log("c", 0);
    }
    {
        Logger logger(p1);
        logger.CleanLogs(2, 0);
        ASSERT_EQ(CountFilesWithExtension(test_dir_, ".log"), 2u);
        logger.CleanLogs(2, 0);
        ASSERT_EQ(CountFilesWithExtension(test_dir_, ".log"), 2u);
    }
}
