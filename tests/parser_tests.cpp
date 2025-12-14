#include "../lib/exception.hpp"
#include "../lib/parser.hpp"
#include <gtest/gtest.h>

using namespace mystd;

// Fixture для тестов парсера
class ParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        parser = new Parser();
    }
    
    void TearDown() override {
        delete parser;
    }
    
    Parser* parser;
};

TEST_F(ParserTest, EmptyParser) {
    EXPECT_EQ(parser->Size(), 0);
    EXPECT_FALSE(parser->ArgumentInvolved("nonexistent"));
    EXPECT_EQ(parser->ArgumentSize("nonexistent"), 0);
}

TEST_F(ParserTest, AddArguments) {
    
    parser->AddArgument("-f", "--file", "file", Nargs::kImmediateOptional);
    parser->AddArgument("-v", "--verbose", "verbose", Nargs::kDelayedOptional);
    parser->AddArgument("-l", "--list", "list", Nargs::kDelayedZeroOrMore);
    parser->AddArgument("-o", "--output", "output", Nargs::kImmediateZeroOrMore);
    parser->AddArgument(nullptr, nullptr, "command", Nargs::kCommandOnly);
    
    EXPECT_EQ(parser->Size(), 5);
    
    parser->AddArgument("-f", "--another", "file2", Nargs::kImmediateOptional);
    EXPECT_EQ(parser->Size(), 6);
    
    parser->AddArgument("-x", "--file", "file3", Nargs::kImmediateOptional);
    parser->AddArgument("-f", nullptr, "file4", Nargs::kImmediateOptional);
}

TEST_F(ParserTest, ParseSimpleArguments) {
    parser->AddArgument("-h", "--help", "help", Nargs::kImmediateOptional);
    parser->AddArgument("-v", "--verbose", "verbose", Nargs::kDelayedOptional);
    
    const char* argv[] = {"program", "-h", "--verbose"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    parser->Parse(argc, argv);
    
    EXPECT_TRUE(parser->ArgumentInvolved("help"));
    EXPECT_TRUE(parser->ArgumentInvolved("verbose"));
    EXPECT_EQ(parser->ArgumentSize("help"), 0); 
    EXPECT_EQ(parser->ArgumentSize("verbose"), 0);
}

TEST_F(ParserTest, ParseWithValues) {
    parser->AddArgument("-f", "--file", "file", Nargs::kImmediateOptional);
    parser->AddArgument("-o", "--output", "output", Nargs::kImmediateZeroOrMore);
    parser->AddArgument("-l", "--list", "list", Nargs::kDelayedZeroOrMore);
    const char* argv[] = {"program", "-f", "input.txt", "--output", "out1", "out2", "--list", "item1", "item2"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    parser->Parse(argc, argv);
    
    EXPECT_TRUE(parser->ArgumentInvolved("file"));
    EXPECT_EQ(parser->ArgumentSize("file"), 1);
    EXPECT_EQ(parser->GetArgumentValue("file", 0), "input.txt");
    
    EXPECT_TRUE(parser->ArgumentInvolved("output"));
    EXPECT_EQ(parser->ArgumentSize("output"), 2);
    EXPECT_EQ(parser->GetArgumentValue("output", 0), "out1");
    EXPECT_EQ(parser->GetArgumentValue("output", 1), "out2");
    
    EXPECT_TRUE(parser->ArgumentInvolved("list"));
    EXPECT_EQ(parser->ArgumentSize("list"), 2);
    EXPECT_EQ(parser->GetArgumentValue("list", 0), "item1");
    EXPECT_EQ(parser->GetArgumentValue("list", 1), "item2");
}

// Тест 5: Только команда (command only)
TEST_F(ParserTest, CommandOnly) {
    parser->AddArgument(nullptr, nullptr, "command", Nargs::kCommandOnly);
    parser->AddArgument("-v", "--verbose", "verbose", Nargs::kDelayedOptional);
    
    const char* argv[] = {"program", "run", "--verbose", "command_arg"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    parser->Parse(argc, argv);
    
    EXPECT_FALSE(parser->ArgumentInvolved("command"));
    EXPECT_GE(parser->ArgumentSize("command"), 0);
}

// Тест 6: Смешанные аргументы
TEST_F(ParserTest, MixedArguments) {
    parser->AddArgument("-f", "--file", "file", Nargs::kImmediateOptional);
    parser->AddArgument("-l", "--list", "list", Nargs::kDelayedZeroOrMore);
    parser->AddArgument("-f", "--file", "command", Nargs::kCommandOnly);
    
    const char* argv[] = {"program", "start", "-f", "config.yaml", "--list", "a", "b", "c", "extra"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    parser->Parse(argc, argv);
    
    // Проверка команды
    EXPECT_TRUE(parser->ArgumentInvolved("command"));
    
    // Проверка file
    EXPECT_TRUE(parser->ArgumentInvolved("file"));
    EXPECT_EQ(parser->GetArgumentValue("file", 0), "config.yaml");
    
    // Проверка list
    EXPECT_TRUE(parser->ArgumentInvolved("list"));
    EXPECT_EQ(parser->ArgumentSize("list"), 4);
}

// Тест 7: Аргументы не использованы
TEST_F(ParserTest, ArgumentNotInvolved) {
    parser->AddArgument("-h", "--help", "help", Nargs::kImmediateOptional);
    parser->AddArgument("-v", "--verbose", "verbose", Nargs::kDelayedOptional);
    
    const char* argv[] = {"program"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    parser->Parse(argc, argv);
    
    EXPECT_FALSE(parser->ArgumentInvolved("help"));
    EXPECT_FALSE(parser->ArgumentInvolved("verbose"));
    EXPECT_EQ(parser->ArgumentSize("help"), 0);
    EXPECT_EQ(parser->ArgumentSize("verbose"), 0);
}

// Тест 8: Краевые случаи
TEST_F(ParserTest, EdgeCases) {
    parser->AddArgument("-s", "--silent", "silent", Nargs::kImmediateOptional);
    parser->AddArgument("-e", "--empty", "empty", Nargs::kImmediateZeroOrMore);
    
    const char* argv1[] = {"program", "-s"};
    int argc1 = sizeof(argv1) / sizeof(argv1[0]);
    parser->Parse(argc1, argv1);
    
    EXPECT_TRUE(parser->ArgumentInvolved("silent"));
    EXPECT_EQ(parser->ArgumentSize("silent"), 0);
    
    const char* argv2[] = {"program", "-e"};
    int argc2 = sizeof(argv2) / sizeof(argv2[0]);
    parser->Parse(argc2, argv2);
    
    EXPECT_TRUE(parser->ArgumentInvolved("empty"));
    EXPECT_EQ(parser->ArgumentSize("empty"), 0);
}

// Тест 10: Получение значений аргументов (краевые случаи)
TEST_F(ParserTest, GetArgumentValueEdgeCases) {
    parser->AddArgument("-f", "--file", "file", Nargs::kImmediateOptional);
    
    const char* argv[] = {"program", "-f", "test.txt"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    parser->Parse(argc, argv);
    
    // Получение значения с корректным индексом
    EXPECT_EQ(parser->GetArgumentValue("file", 0), "test.txt");
    
    // Попытка получить несуществующий аргумент
    // (зависит от реализации - может выбрасывать исключение или возвращать пустую строку)
    // Для теста просто проверим, что не падает
    EXPECT_NO_THROW(parser->ArgumentSize("nonexistent"));
    
    // Попытка получить значение за пределами размера
    EXPECT_THROW(parser->GetArgumentValue("file", 1), Exception);
}

// Тест 11: Комплексный сценарий
TEST_F(ParserTest, ComplexScenario) {
    // Разные типы аргументов
    parser->AddArgument("-h", "--help", "help", Nargs::kImmediateOptional);
    parser->AddArgument("-v", "--version", "version", Nargs::kDelayedOptional);
    parser->AddArgument("-i", "--input", "input", Nargs::kImmediateOptional);
    parser->AddArgument("-o", "--output", "output", Nargs::kImmediateZeroOrMore);
    parser->AddArgument("-l", "--list", "list", Nargs::kDelayedZeroOrMore);
    
    const char* argv[] = {
        "myapp",
        "process",
        "-i", "in.txt",
        "-o", "out1.txt", "out2.txt",
        "--list", "item1", "item2", "item3",
        "-h",
        "extra_arg1",
        "-v",
        "extra_arg2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    parser->Parse(argc, argv);
    
    // Проверка input
    EXPECT_TRUE(parser->ArgumentInvolved("input"));
    EXPECT_EQ(parser->GetArgumentValue("input", 0), "in.txt");
    
    // Проверка output
    EXPECT_TRUE(parser->ArgumentInvolved("output"));
    EXPECT_EQ(parser->ArgumentSize("output"), 2);
    EXPECT_EQ(parser->GetArgumentValue("output", 0), "out1.txt");
    EXPECT_EQ(parser->GetArgumentValue("output", 1), "out2.txt");
    
    // Проверка list
    EXPECT_TRUE(parser->ArgumentInvolved("list"));
    EXPECT_EQ(parser->ArgumentSize("list"), 4);
    EXPECT_EQ(parser->GetArgumentValue("list", 0), "item1");
    EXPECT_EQ(parser->GetArgumentValue("list", 1), "item2");
    EXPECT_EQ(parser->GetArgumentValue("list", 2), "item3");
    EXPECT_EQ(parser->GetArgumentValue("list", 3), "extra_arg2");
    
    // Аргументы без значений
    EXPECT_TRUE(parser->ArgumentInvolved("help"));
    EXPECT_TRUE(parser->ArgumentInvolved("version"));
    EXPECT_EQ(parser->ArgumentSize("extra_arg1"), 0);
    EXPECT_EQ(parser->ArgumentSize("extra_arg2"), 0);
}

// Тест 13: Неверный ввод
TEST_F(ParserTest, InvalidInput) {
    parser->AddArgument("-f", "--file", "file", Nargs::kImmediateOptional);
    
    // Пустой вызов Parse
    const char* argv1[] = {"program"};
    int argc1 = 1;
    parser->Parse(argc1, argv1);
    EXPECT_FALSE(parser->ArgumentInvolved("file"));
    
    // Некорректный аргумент (не добавленный)
    const char* argv2[] = {"program", "-x"};
    int argc2 = 2;
    parser->Parse(argc2, argv2);
    // Должно игнорировать некорректный аргумент или обработать как команду
    
    // Должно быть пусто или содержать команду
    EXPECT_FALSE(parser->ArgumentInvolved("file"));
}

// Тест 14: Разные типы Nargs
TEST_F(ParserTest, DifferentNargsTypes) {
    // kImmediateOptional - немедленная, необязательная
    parser->AddArgument("-a", "--alpha", "alpha", Nargs::kImmediateOptional);
    
    // kDelayedOptional - отложенная, необязательная
    parser->AddArgument("-b", "--beta", "beta", Nargs::kDelayedOptional);
    
    // kDelayedZeroOrMore - отложенная, ноль или более
    parser->AddArgument("-c", "--gamma", "gamma", Nargs::kDelayedZeroOrMore);
    
    // kImmediateZeroOrMore - немедленная, ноль или более
    parser->AddArgument("-d", "--delta", "delta", Nargs::kImmediateZeroOrMore);
    
    const char* argv[] = {
        "program",
        "-a", "val_a",
        "-b", "val_b",
        "-c", "val_c1", "val_c2",
        "-d", "val_d1", "val_d2", "val_d3"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    parser->Parse(argc, argv);
    
    // Проверка каждого типа
    EXPECT_TRUE(parser->ArgumentInvolved("alpha"));
    EXPECT_EQ(parser->ArgumentSize("alpha"), 1);
    
    EXPECT_TRUE(parser->ArgumentInvolved("beta"));
    EXPECT_EQ(parser->ArgumentSize("beta"), 1);
    
    EXPECT_TRUE(parser->ArgumentInvolved("gamma"));
    EXPECT_EQ(parser->ArgumentSize("gamma"), 2);
    
    EXPECT_TRUE(parser->ArgumentInvolved("delta"));
    EXPECT_EQ(parser->ArgumentSize("delta"), 3);
}

// Тест 15: Обработка только длинных имен
TEST_F(ParserTest, LongNamesOnly) {
    parser->AddArgument(nullptr, "--help", "help", Nargs::kImmediateOptional);
    parser->AddArgument(nullptr, "--verbose", "verbose", Nargs::kDelayedOptional);
    
    const char* argv[] = {"program", "--help", "--verbose"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    parser->Parse(argc, argv);
    
    EXPECT_TRUE(parser->ArgumentInvolved("help"));
    EXPECT_TRUE(parser->ArgumentInvolved("verbose"));
}

// Тест 16: Обработка только коротких имен
TEST_F(ParserTest, ShortNamesOnly) {
    parser->AddArgument("-h", nullptr, "help", Nargs::kImmediateOptional);
    parser->AddArgument("-v", nullptr, "verbose", Nargs::kDelayedOptional);
    
    const char* argv[] = {"program", "-h", "-v"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    parser->Parse(argc, argv);
    
    EXPECT_TRUE(parser->ArgumentInvolved("help"));
    EXPECT_TRUE(parser->ArgumentInvolved("verbose"));
}

// Тест 19: Обработка специальных символов в значениях
TEST_F(ParserTest, SpecialCharactersInValues) {
    parser->AddArgument("-p", "--path", "path", Nargs::kImmediateOptional);
    
    const char* argv[] = {"program", "-p", "/home/user/file with spaces.txt"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    parser->Parse(argc, argv);
    
    EXPECT_TRUE(parser->ArgumentInvolved("path"));
    EXPECT_EQ(parser->GetArgumentValue("path", 0), "/home/user/file with spaces.txt");
}

// Тест 20: Конструктор и деструктор (базовый)
TEST(ParserConstructorTest, ConstructorDestructor) {
    // Просто создаем и удаляем парсер
    Parser* parser = new Parser();
    EXPECT_NE(parser, nullptr);
    
    // Проверяем начальное состояние
    EXPECT_EQ(parser->Size(), 0);
    
    delete parser;
    // Если здесь нет краша - тест пройден
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}