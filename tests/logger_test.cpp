#include <gtest/gtest.h>
#include "../lib/loggers/logger.hpp"
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>

namespace fs = std::filesystem;

class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем временную директорию для тестовых файлов
        test_dir_ = fs::temp_directory_path() / "logger_test";
        fs::create_directories(test_dir_);
    }
    
    void TearDown() override {
        // Удаляем временную директорию и все файлы
        if (fs::exists(test_dir_)) {
            fs::remove_all(test_dir_);
        }
    }
    
    std::string ReadFileContent(const fs::path& path) {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            return "";
        }
        
        std::ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    
    fs::path GetTestFilePath(const std::string& filename) {
        return test_dir_ / filename;
    }
    
    void CreateFileWithContent(const fs::path& path, const std::string& content) {
        std::ofstream file(path, std::ios::binary);
        if (file.is_open()) {
            file << content;
        }
    }
    
    fs::path test_dir_;
};

// ============================================================================
// Группа 1: ConstructorTests - тестирование конструктора
// ============================================================================

TEST_F(LoggerTest, ConstructorTests_NewFile) {
    fs::path test_file = GetTestFilePath("new_file.log");
    
    // Создание логгера с новым файлом
    Logger logger(test_file);
    
    // Файл должен быть создан и очищен
    ASSERT_TRUE(fs::exists(test_file));
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "");
}

TEST_F(LoggerTest, ConstructorTests_ExistingFile) {
    fs::path test_file = GetTestFilePath("existing_file.log");
    
    // Создаем файл с содержимым
    CreateFileWithContent(test_file, "Previous content\n");
    
    // Создание логгера с существующим файлом
    Logger logger(test_file);
    
    // Файл должен быть очищен
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "");
}

TEST_F(LoggerTest, ConstructorTests_Subdirectory) {
    fs::path subdir = test_dir_ / "subdir";
    fs::path test_file = subdir / "nested.log";
    
    // Создание логгера с файлом в поддиректории
    Logger logger(test_file);
    
    // Директория и файл должны быть созданы
    ASSERT_TRUE(fs::exists(subdir));
    ASSERT_TRUE(fs::exists(test_file));
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "");
}

TEST_F(LoggerTest, ConstructorTests_RelativePath) {
    fs::path test_file = "relative_test.log";
    
    // Создание логгера с относительным путем
    Logger logger(test_file);
    
    // Файл должен быть создан
    ASSERT_TRUE(fs::exists(test_file));
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "");
    
    // Очистка
    fs::remove(test_file);
}

TEST_F(LoggerTest, ConstructorTests_AbsolutePath) {
    fs::path test_file = GetTestFilePath("absolute.log");
    fs::path absolute_path = fs::absolute(test_file);
    
    // Создание логгера с абсолютным путем
    Logger logger(absolute_path);
    
    // Файл должен быть создан
    ASSERT_TRUE(fs::exists(absolute_path));
    std::string content = ReadFileContent(absolute_path);
    ASSERT_EQ(content, "");
}

// ============================================================================
// Группа 2: FileClearingTests - тестирование очистки файла при создании
// ============================================================================

TEST_F(LoggerTest, FileClearingTests_EmptyFile) {
    fs::path test_file = GetTestFilePath("empty.log");
    
    // Создаем пустой файл
    std::ofstream(test_file).close();
    
    // Создание логгера должно очистить файл
    Logger logger(test_file);
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "");
}

TEST_F(LoggerTest, FileClearingTests_FileWithContent) {
    fs::path test_file = GetTestFilePath("with_content.log");
    
    // Создаем файл с содержимым
    CreateFileWithContent(test_file, "Some content here\nMore lines\n");
    
    // Создание логгера должно очистить файл
    Logger logger(test_file);
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "");
}

TEST_F(LoggerTest, FileClearingTests_AfterPreviousLogs) {
    fs::path test_file = GetTestFilePath("after_logs.log");
    
    // Создаем логгер и логируем что-то
    {
        Logger logger(test_file);
        logger.Log("First message");
    }
    
    // Создаем новый логгер - файл должен быть очищен
    Logger logger2(test_file);
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "");
}

TEST_F(LoggerTest, FileClearingTests_RecreateLogger) {
    fs::path test_file = GetTestFilePath("recreate.log");
    
    // Первый логгер
    {
        Logger logger1(test_file);
        logger1.Log("Message 1");
    }
    
    // Второй логгер - должен очистить файл
    {
        Logger logger2(test_file);
        std::string content = ReadFileContent(test_file);
        ASSERT_EQ(content, "");
        
        logger2.Log("Message 2");
        content = ReadFileContent(test_file);
        ASSERT_EQ(content, "Message 2\n");
    }
}

TEST_F(LoggerTest, FileClearingTests_LargeContent) {
    fs::path test_file = GetTestFilePath("large.log");
    
    // Создаем файл с большим содержимым
    std::string large_content(10000, 'A');
    CreateFileWithContent(test_file, large_content);
    
    // Создание логгера должно очистить файл
    Logger logger(test_file);
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "");
}

// ============================================================================
// Группа 3: SingleLogTests - тестирование одиночных операций логирования
// ============================================================================

TEST_F(LoggerTest, SingleLogTests_SimpleMessage) {
    fs::path test_file = GetTestFilePath("simple.log");
    Logger logger(test_file);
    
    logger.Log("Hello World");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "Hello World\n");
}

TEST_F(LoggerTest, SingleLogTests_EmptyMessage) {
    fs::path test_file = GetTestFilePath("empty_msg.log");
    Logger logger(test_file);
    
    logger.Log("");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "\n");
}

TEST_F(LoggerTest, SingleLogTests_MessageWithSpaces) {
    fs::path test_file = GetTestFilePath("spaces.log");
    Logger logger(test_file);
    
    logger.Log("  Message with spaces  ");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "  Message with spaces  \n");
}

TEST_F(LoggerTest, SingleLogTests_MessageWithNewlines) {
    fs::path test_file = GetTestFilePath("newlines.log");
    Logger logger(test_file);
    
    logger.Log("Line1\nLine2\nLine3");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "Line1\nLine2\nLine3\n");
}

TEST_F(LoggerTest, SingleLogTests_LongMessage) {
    fs::path test_file = GetTestFilePath("long.log");
    Logger logger(test_file);
    
    std::string long_message(1000, 'X');
    logger.Log(long_message);
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, long_message + "\n");
}

// ============================================================================
// Группа 4: MultipleLogTests - тестирование множественных операций логирования
// ============================================================================

TEST_F(LoggerTest, MultipleLogTests_SequentialMessages) {
    fs::path test_file = GetTestFilePath("sequential.log");
    Logger logger(test_file);
    
    logger.Log("First");
    logger.Log("Second");
    logger.Log("Third");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "First\nSecond\nThird\n");
}

TEST_F(LoggerTest, MultipleLogTests_SameMessages) {
    fs::path test_file = GetTestFilePath("same.log");
    Logger logger(test_file);
    
    logger.Log("Same");
    logger.Log("Same");
    logger.Log("Same");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "Same\nSame\nSame\n");
}

TEST_F(LoggerTest, MultipleLogTests_DifferentLengths) {
    fs::path test_file = GetTestFilePath("different_lengths.log");
    Logger logger(test_file);
    
    logger.Log("A");
    logger.Log("This is a longer message");
    logger.Log("Medium");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "A\nThis is a longer message\nMedium\n");
}

TEST_F(LoggerTest, MultipleLogTests_ManyMessages) {
    fs::path test_file = GetTestFilePath("many.log");
    Logger logger(test_file);
    
    for (int i = 0; i < 100; ++i) {
        logger.Log("Message " + std::to_string(i));
    }
    
    std::string content = ReadFileContent(test_file);
    std::string expected;
    for (int i = 0; i < 100; ++i) {
        expected += "Message " + std::to_string(i) + "\n";
    }
    ASSERT_EQ(content, expected);
}

TEST_F(LoggerTest, MultipleLogTests_AlternatingLengths) {
    fs::path test_file = GetTestFilePath("alternating.log");
    Logger logger(test_file);
    
    logger.Log("Short");
    logger.Log("This is a much longer message that contains many words");
    logger.Log("A");
    logger.Log("Another very long message with lots of content");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "Short\nThis is a much longer message that contains many words\nA\nAnother very long message with lots of content\n");
}

// ============================================================================
// Группа 5: MessageContentTests - тестирование различного содержимого сообщений
// ============================================================================

TEST_F(LoggerTest, MessageContentTests_SpecialCharacters) {
    fs::path test_file = GetTestFilePath("special.log");
    Logger logger(test_file);
    
    logger.Log("!@#$%^&*()_+-=[]{}|;':\",./<>?");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "!@#$%^&*()_+-=[]{}|;':\",./<>?\n");
}

TEST_F(LoggerTest, MessageContentTests_Numbers) {
    fs::path test_file = GetTestFilePath("numbers.log");
    Logger logger(test_file);
    
    logger.Log("1234567890");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "1234567890\n");
}

TEST_F(LoggerTest, MessageContentTests_MixedContent) {
    fs::path test_file = GetTestFilePath("mixed.log");
    Logger logger(test_file);
    
    logger.Log("Hello123!@# World456");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "Hello123!@# World456\n");
}

TEST_F(LoggerTest, MessageContentTests_TabCharacter) {
    fs::path test_file = GetTestFilePath("tab.log");
    Logger logger(test_file);
    
    logger.Log("Column1\tColumn2\tColumn3");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "Column1\tColumn2\tColumn3\n");
}

TEST_F(LoggerTest, MessageContentTests_EscapedCharacters) {
    fs::path test_file = GetTestFilePath("escaped.log");
    Logger logger(test_file);
    
    logger.Log("\\n\\t\\r\\\"\\'");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "\\n\\t\\r\\\"\\'\n");
}

// ============================================================================
// Группа 6: SequentialLoggingTests - тестирование последовательного логирования
// ============================================================================

TEST_F(LoggerTest, SequentialLoggingTests_AfterClearing) {
    fs::path test_file = GetTestFilePath("after_clear.log");
    
    {
        Logger logger(test_file);
        logger.Log("Before clear");
    }
    
    {
        Logger logger(test_file);
        logger.Log("After clear");
    }
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "After clear\n");
}

TEST_F(LoggerTest, SequentialLoggingTests_WithGaps) {
    fs::path test_file = GetTestFilePath("gaps.log");
    Logger logger(test_file);
    
    logger.Log("First");
    // Промежуток (имитация)
    logger.Log("Second");
    logger.Log("Third");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "First\nSecond\nThird\n");
}

TEST_F(LoggerTest, SequentialLoggingTests_InLoop) {
    fs::path test_file = GetTestFilePath("loop.log");
    Logger logger(test_file);
    
    for (int i = 1; i <= 10; ++i) {
        logger.Log("Iteration " + std::to_string(i));
    }
    
    std::string content = ReadFileContent(test_file);
    std::string expected;
    for (int i = 1; i <= 10; ++i) {
        expected += "Iteration " + std::to_string(i) + "\n";
    }
    ASSERT_EQ(content, expected);
}

TEST_F(LoggerTest, SequentialLoggingTests_Accumulation) {
    fs::path test_file = GetTestFilePath("accumulation.log");
    Logger logger(test_file);
    
    std::string accumulated;
    for (int i = 0; i < 5; ++i) {
        std::string msg = "Part" + std::to_string(i);
        logger.Log(msg);
        accumulated += msg + "\n";
    }
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, accumulated);
}

TEST_F(LoggerTest, SequentialLoggingTests_OrderVerification) {
    fs::path test_file = GetTestFilePath("order.log");
    Logger logger(test_file);
    
    logger.Log("1");
    logger.Log("2");
    logger.Log("3");
    logger.Log("4");
    logger.Log("5");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "1\n2\n3\n4\n5\n");
    
    // Проверяем порядок
    size_t pos1 = content.find("1\n");
    size_t pos2 = content.find("2\n");
    size_t pos3 = content.find("3\n");
    size_t pos4 = content.find("4\n");
    size_t pos5 = content.find("5\n");
    
    ASSERT_LT(pos1, pos2);
    ASSERT_LT(pos2, pos3);
    ASSERT_LT(pos3, pos4);
    ASSERT_LT(pos4, pos5);
}

// ============================================================================
// Группа 7: FilePathTests - тестирование различных путей к файлам
// ============================================================================

TEST_F(LoggerTest, FilePathTests_CurrentDirectory) {
    fs::path test_file = "current_dir_test.log";
    
    Logger logger(test_file);
    logger.Log("Test");
    
    ASSERT_TRUE(fs::exists(test_file));
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "Test\n");
    
    fs::remove(test_file);
}

TEST_F(LoggerTest, FilePathTests_Subdirectory) {
    fs::path subdir = test_dir_ / "logs";
    fs::path test_file = subdir / "subdir_test.log";
    
    Logger logger(test_file);
    logger.Log("Subdirectory test");
    
    ASSERT_TRUE(fs::exists(subdir));
    ASSERT_TRUE(fs::exists(test_file));
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "Subdirectory test\n");
}

TEST_F(LoggerTest, FilePathTests_CreateDirectories) {
    fs::path deep_path = test_dir_ / "level1" / "level2" / "level3" / "deep.log";
    
    Logger logger(deep_path);
    logger.Log("Deep test");
    
    ASSERT_TRUE(fs::exists(deep_path));
    std::string content = ReadFileContent(deep_path);
    ASSERT_EQ(content, "Deep test\n");
}

TEST_F(LoggerTest, FilePathTests_LogExtension) {
    fs::path test_file = GetTestFilePath("test.log");
    
    Logger logger(test_file);
    logger.Log("Log extension test");
    
    ASSERT_TRUE(fs::exists(test_file));
    ASSERT_EQ(test_file.extension(), ".log");
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "Log extension test\n");
}

TEST_F(LoggerTest, FilePathTests_TxtExtension) {
    fs::path test_file = GetTestFilePath("test.txt");
    
    Logger logger(test_file);
    logger.Log("Txt extension test");
    
    ASSERT_TRUE(fs::exists(test_file));
    ASSERT_EQ(test_file.extension(), ".txt");
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "Txt extension test\n");
}

// ============================================================================
// Группа 8: EdgeCaseTests - тестирование граничных случаев
// ============================================================================

TEST_F(LoggerTest, EdgeCaseTests_VeryLongMessage) {
    fs::path test_file = GetTestFilePath("very_long.log");
    Logger logger(test_file);
    
    std::string very_long(100000, 'A');
    logger.Log(very_long);
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, very_long + "\n");
    ASSERT_EQ(content.size(), very_long.size() + 1);
}

TEST_F(LoggerTest, EdgeCaseTests_OnlySpaces) {
    fs::path test_file = GetTestFilePath("spaces_only.log");
    Logger logger(test_file);
    
    logger.Log("     ");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "     \n");
}

TEST_F(LoggerTest, EdgeCaseTests_OnlyNewlines) {
    fs::path test_file = GetTestFilePath("newlines_only.log");
    Logger logger(test_file);
    
    logger.Log("\n\n\n");
    
    std::string content = ReadFileContent(test_file);
    ASSERT_EQ(content, "\n\n\n\n");
}

TEST_F(LoggerTest, EdgeCaseTests_MultipleOperations) {
    fs::path test_file = GetTestFilePath("multiple_ops.log");
    Logger logger(test_file);
    
    // Множественные операции
    for (int i = 0; i < 50; ++i) {
        logger.Log("Message " + std::to_string(i));
    }
    
    std::string content = ReadFileContent(test_file);
    ASSERT_GT(content.size(), 0);
    
    // Проверяем, что все сообщения на месте
    for (int i = 0; i < 50; ++i) {
        std::string expected = "Message " + std::to_string(i) + "\n";
        ASSERT_NE(content.find(expected), std::string::npos);
    }
}

TEST_F(LoggerTest, EdgeCaseTests_NewlineFormat) {
    fs::path test_file = GetTestFilePath("format.log");
    Logger logger(test_file);
    
    logger.Log("Test message");
    
    std::string content = ReadFileContent(test_file);
    
    // Проверяем, что после сообщения добавлен \n
    ASSERT_EQ(content.back(), '\n');
    ASSERT_EQ(content, "Test message\n");
    
    // Проверяем, что каждый Log добавляет \n
    logger.Log("Another message");
    content = ReadFileContent(test_file);
    ASSERT_EQ(content, "Test message\nAnother message\n");
    ASSERT_EQ(content.back(), '\n');
}
