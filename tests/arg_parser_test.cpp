#include <gtest/gtest.h>
#include "../lib/parsers/arg_parser.hpp"
#include "../lib/exception.hpp"
#include <vector>
#include <string>

using namespace mystd;

class ArgParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Настройка перед каждым тестом
        test_args_.clear();
        test_argv_.clear();
    }
    
    void TearDown() override {
        // Очистка после каждого теста
        test_args_.clear();
        test_argv_.clear();
    }
    
    // Вспомогательная функция для создания argv из вектора строк
    std::vector<const char*> CreateArgv(const std::vector<std::string>& args) {
        test_args_.clear();
        test_argv_.clear();
        
        test_argv_.push_back("program"); // argv[0] - имя программы
        for (const auto& arg : args) {
            test_args_.push_back(arg); // Сохраняем строки
            test_argv_.push_back(test_args_.back().c_str()); // Используем указатели на сохраненные строки
        }
        return test_argv_;
    }
    
private:
    std::vector<std::string> test_args_; // Хранилище строк для валидности указателей
    std::vector<const char*> test_argv_; // Вектор указателей
};

// ============================================================================
// Группа 1: ConstructorAndBasicTests - тестирование конструктора и базовых методов
// ============================================================================

TEST_F(ArgParserTest, ConstructorAndBasicTests_DefaultConstructor) {
    Parser parser;
    
    // Проверка начального состояния
    ASSERT_EQ(parser.Size(), 0);
    ASSERT_FALSE(parser.ArgumentInvolved("test"));
    ASSERT_EQ(parser.ArgumentSize("test"), 0);
}

TEST_F(ArgParserTest, ConstructorAndBasicTests_SizeAfterAdd) {
    Parser parser;
    
    // Добавляем аргументы и проверяем Size()
    parser.AddArgument("-a", "--arg1", "arg1", kImmediateOptional);
    ASSERT_EQ(parser.Size(), 1);
    
    parser.AddArgument("-b", "--arg2", "arg2", kDelayedOptional);
    ASSERT_EQ(parser.Size(), 2);
    
    parser.AddArgument("-c", "--arg3", "arg3", kCommandOnly);
    ASSERT_EQ(parser.Size(), 3);
}

TEST_F(ArgParserTest, ConstructorAndBasicTests_EmptyParser) {
    Parser parser;
    
    // Работа с пустым парсером
    auto argv = CreateArgv({});
    parser.Parse(1, argv.data());
    
    ASSERT_EQ(parser.Size(), 0);
    ASSERT_FALSE(parser.ArgumentInvolved("nonexistent"));
    ASSERT_EQ(parser.ArgumentSize("nonexistent"), 0);
}

TEST_F(ArgParserTest, ConstructorAndBasicTests_MultipleArguments) {
    Parser parser;
    
    // Добавление нескольких аргументов
    parser.AddArgument("-a", "--alpha", "alpha", kImmediateOptional);
    parser.AddArgument("-b", "--beta", "beta", kDelayedOptional);
    parser.AddArgument("-c", "--gamma", "gamma", kDelayedZeroOrMore);
    parser.AddArgument("-d", "--delta", "delta", kImmediateZeroOrMore);
    parser.AddArgument("-e", "--epsilon", "epsilon", kCommandOnly);
    
    ASSERT_EQ(parser.Size(), 5);
}

TEST_F(ArgParserTest, ConstructorAndBasicTests_Destructor) {
    // Проверка корректного освобождения памяти
    {
        Parser parser;
        parser.AddArgument("-a", "--arg1", "arg1", kImmediateOptional);
        parser.AddArgument("-b", "--arg2", "arg2", kDelayedOptional);
        parser.AddArgument("-c", "--arg3", "arg3", kDelayedZeroOrMore);
    }
    // Если деструктор работает корректно, не должно быть утечек памяти
    // В реальных тестах можно использовать инструменты для проверки утечек
    ASSERT_TRUE(true);
}

// ============================================================================
// Группа 2: AddArgumentTests - тестирование метода AddArgument
// ============================================================================

TEST_F(ArgParserTest, AddArgumentTests_ShortAndLongNames) {
    Parser parser;
    
    // Добавление аргумента с коротким и длинным именем
    parser.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    
    ASSERT_EQ(parser.Size(), 1);
    ASSERT_TRUE(parser.ArgumentInvolved("verbose"));
    
    auto argv = CreateArgv({"-v"});
    parser.Parse(2, argv.data());
    ASSERT_TRUE(parser.ArgumentInvolved("verbose"));
    
    auto argv2 = CreateArgv({"--verbose"});
    Parser parser2;
    parser2.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    parser2.Parse(2, argv2.data());
    ASSERT_TRUE(parser2.ArgumentInvolved("verbose"));
}

TEST_F(ArgParserTest, AddArgumentTests_OnlyShortName) {
    Parser parser;
    
    // Добавление только с коротким именем (длинное имя может быть пустым)
    parser.AddArgument("-a", "", "alpha", kCommandOnly);
    
    ASSERT_EQ(parser.Size(), 1);
    
    auto argv = CreateArgv({"-a"});
    parser.Parse(2, argv.data());
    ASSERT_TRUE(parser.ArgumentInvolved("alpha"));
}

TEST_F(ArgParserTest, AddArgumentTests_OnlyLongName) {
    Parser parser;
    
    // Добавление только с длинным именем (короткое имя может быть пустым)
    parser.AddArgument("", "--alpha", "alpha", kCommandOnly);
    
    ASSERT_EQ(parser.Size(), 1);
    
    auto argv = CreateArgv({"--alpha"});
    parser.Parse(2, argv.data());
    ASSERT_TRUE(parser.ArgumentInvolved("alpha"));
}

TEST_F(ArgParserTest, AddArgumentTests_AllNargsTypes) {
    Parser parser;
    
    // Добавление аргументов со всеми типами Nargs
    parser.AddArgument("-a", "--immediate_opt", "immediate_opt", kImmediateOptional);
    parser.AddArgument("-b", "--delayed_opt", "delayed_opt", kDelayedOptional);
    parser.AddArgument("-c", "--delayed_zero", "delayed_zero", kDelayedZeroOrMore);
    parser.AddArgument("-d", "--immediate_zero", "immediate_zero", kImmediateZeroOrMore);
    parser.AddArgument("-e", "--command", "command", kCommandOnly);
    parser.AddArgument("-f", "--remaining", "remaining", kRemainingArguments);
    
    ASSERT_EQ(parser.Size(), 6);
}

TEST_F(ArgParserTest, AddArgumentTests_EmptyNames) {
    Parser parser;
    
    // Добавление с пустыми строками в именах
    parser.AddArgument("", "", "empty_names", kCommandOnly);
    parser.AddArgument("", "--only_long", "only_long", kCommandOnly);
    parser.AddArgument("-s", "", "only_short", kCommandOnly);
    
    ASSERT_EQ(parser.Size(), 3);
}

// ============================================================================
// Группа 3: ParseShortArgumentsTests - тестирование парсинга коротких аргументов
// ============================================================================

TEST_F(ArgParserTest, ParseShortArgumentsTests_SimpleFlag) {
    Parser parser;
    parser.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    
    // Простой флаг без значения
    auto argv = CreateArgv({"-v"});
    parser.Parse(2, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("verbose"));
}

TEST_F(ArgParserTest, ParseShortArgumentsTests_WithEquals) {
    Parser parser;
    parser.AddArgument("-f", "--file", "file", kImmediateOptional);
    
    // Аргумент со знаком равенства
    auto argv = CreateArgv({"-f=test.txt"});
    parser.Parse(2, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("file"));
    ASSERT_EQ(parser.ArgumentSize("file"), 1);
    ASSERT_EQ(parser.GetArgumentValue("file", 0), "test.txt");
}

TEST_F(ArgParserTest, ParseShortArgumentsTests_MultipleFlags) {
    Parser parser;
    parser.AddArgument("-a", "--alpha", "alpha", kCommandOnly);
    parser.AddArgument("-b", "--beta", "beta", kCommandOnly);
    parser.AddArgument("-c", "--gamma", "gamma", kCommandOnly);
    
    // Несколько флагов подряд
    auto argv = CreateArgv({"-a", "-b", "-c"});
    parser.Parse(4, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("alpha"));
    ASSERT_TRUE(parser.ArgumentInvolved("beta"));
    ASSERT_TRUE(parser.ArgumentInvolved("gamma"));
}

TEST_F(ArgParserTest, ParseShortArgumentsTests_MixedWithValues) {
    Parser parser;
    parser.AddArgument("-f", "--file", "file", kImmediateOptional);
    parser.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    parser.AddArgument("-o", "--output", "output", kImmediateOptional);
    
    // Смешанные флаги и значения
    auto argv = CreateArgv({"-f=input.txt", "-v", "-o=output.txt"});
    parser.Parse(4, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("file"));
    ASSERT_EQ(parser.GetArgumentValue("file", 0), "input.txt");
    ASSERT_TRUE(parser.ArgumentInvolved("verbose"));
    ASSERT_TRUE(parser.ArgumentInvolved("output"));
    ASSERT_EQ(parser.GetArgumentValue("output", 0), "output.txt");
}

TEST_F(ArgParserTest, ParseShortArgumentsTests_NotRegistered) {
    Parser parser;
    parser.AddArgument("-a", "--alpha", "alpha", kCommandOnly);
    
    // Несуществующий короткий аргумент
    auto argv = CreateArgv({"-x"});
    parser.Parse(2, argv.data());
    
    // Не должен быть обработан
    ASSERT_FALSE(parser.ArgumentInvolved("alpha"));
    ASSERT_FALSE(parser.ArgumentInvolved("x"));
}

// ============================================================================
// Группа 4: ParseLongArgumentsTests - тестирование парсинга длинных аргументов
// ============================================================================

TEST_F(ArgParserTest, ParseLongArgumentsTests_SimpleFlag) {
    Parser parser;
    parser.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    
    // Простой флаг без значения
    auto argv = CreateArgv({"--verbose"});
    parser.Parse(2, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("verbose"));
}

TEST_F(ArgParserTest, ParseLongArgumentsTests_WithEquals) {
    Parser parser;
    parser.AddArgument("-f", "--file", "file", kImmediateOptional);
    
    // Аргумент со знаком равенства
    auto argv = CreateArgv({"--file=test.txt"});
    parser.Parse(2, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("file"));
    ASSERT_EQ(parser.ArgumentSize("file"), 1);
    ASSERT_EQ(parser.GetArgumentValue("file", 0), "test.txt");
}

TEST_F(ArgParserTest, ParseLongArgumentsTests_MultipleFlags) {
    Parser parser;
    parser.AddArgument("-a", "--alpha", "alpha", kCommandOnly);
    parser.AddArgument("-b", "--beta", "beta", kCommandOnly);
    parser.AddArgument("-c", "--gamma", "gamma", kCommandOnly);
    
    // Несколько длинных флагов
    auto argv = CreateArgv({"--alpha", "--beta", "--gamma"});
    parser.Parse(4, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("alpha"));
    ASSERT_TRUE(parser.ArgumentInvolved("beta"));
    ASSERT_TRUE(parser.ArgumentInvolved("gamma"));
}

TEST_F(ArgParserTest, ParseLongArgumentsTests_MixedWithShort) {
    Parser parser;
    parser.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    parser.AddArgument("-f", "--file", "file", kImmediateOptional);
    
    // Смешивание коротких и длинных аргументов
    auto argv = CreateArgv({"-v", "--file=test.txt"});
    parser.Parse(3, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("verbose"));
    ASSERT_TRUE(parser.ArgumentInvolved("file"));
    ASSERT_EQ(parser.GetArgumentValue("file", 0), "test.txt");
}

TEST_F(ArgParserTest, ParseLongArgumentsTests_NotRegistered) {
    Parser parser;
    parser.AddArgument("-a", "--alpha", "alpha", kCommandOnly);
    
    // Несуществующий длинный аргумент
    auto argv = CreateArgv({"--unknown"});
    parser.Parse(2, argv.data());
    
    // Не должен быть обработан
    ASSERT_FALSE(parser.ArgumentInvolved("alpha"));
    ASSERT_FALSE(parser.ArgumentInvolved("unknown"));
}

// ============================================================================
// Группа 5: NargsOptionalTests - тестирование ImmediateOptional и DelayedOptional
// ============================================================================

TEST_F(ArgParserTest, NargsOptionalTests_ImmediateOptionalWithValue) {
    Parser parser;
    parser.AddArgument("-f", "--file", "file", kImmediateOptional);
    
    // ImmediateOptional со значением через =
    auto argv = CreateArgv({"-f=test.txt"});
    parser.Parse(2, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("file"));
    ASSERT_EQ(parser.ArgumentSize("file"), 1);
    ASSERT_EQ(parser.GetArgumentValue("file", 0), "test.txt");
}

TEST_F(ArgParserTest, NargsOptionalTests_ImmediateOptionalSeparate) {
    Parser parser;
    parser.AddArgument("-f", "--file", "file", kImmediateOptional);
    
    // ImmediateOptional с отдельным значением
    auto argv = CreateArgv({"-f", "test.txt"});
    parser.Parse(3, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("file"));
    ASSERT_EQ(parser.ArgumentSize("file"), 1);
    ASSERT_EQ(parser.GetArgumentValue("file", 0), "test.txt");
}

TEST_F(ArgParserTest, NargsOptionalTests_ImmediateOptionalWithoutValue) {
    Parser parser;
    parser.AddArgument("-f", "--file", "file", kImmediateOptional);
    parser.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    
    // ImmediateOptional без значения
    auto argv = CreateArgv({"-f", "-v"});
    parser.Parse(3, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("file"));
    ASSERT_EQ(parser.ArgumentSize("file"), 0); // Не было значения
    ASSERT_TRUE(parser.ArgumentInvolved("verbose"));
}

TEST_F(ArgParserTest, NargsOptionalTests_DelayedOptionalWithValue) {
    Parser parser;
    parser.AddArgument("-o", "--output", "output", kDelayedOptional);
    
    // DelayedOptional со значением
    auto argv = CreateArgv({"-o", "result.txt"});
    parser.Parse(3, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("output"));
    ASSERT_EQ(parser.ArgumentSize("output"), 1);
    ASSERT_EQ(parser.GetArgumentValue("output", 0), "result.txt");
}

TEST_F(ArgParserTest, NargsOptionalTests_DelayedOptionalWithoutValue) {
    Parser parser;
    parser.AddArgument("-o", "--output", "output", kDelayedOptional);
    parser.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    
    // DelayedOptional без значения
    auto argv = CreateArgv({"-o", "-v"});
    parser.Parse(3, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("output"));
    ASSERT_EQ(parser.ArgumentSize("output"), 0); // Не было значения
    ASSERT_TRUE(parser.ArgumentInvolved("verbose"));
}

// ============================================================================
// Группа 6: NargsZeroOrMoreTests - тестирование ImmediateZeroOrMore и DelayedZeroOrMore
// ============================================================================

TEST_F(ArgParserTest, NargsZeroOrMoreTests_ImmediateZeroOrMoreMultiple) {
    Parser parser;
    parser.AddArgument("-f", "--files", "files", kImmediateZeroOrMore);
    
    // Несколько значений для ImmediateZeroOrMore
    auto argv = CreateArgv({"-f", "file1.txt", "file2.txt", "file3.txt"});
    parser.Parse(5, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("files"));
    ASSERT_EQ(parser.ArgumentSize("files"), 3);
    ASSERT_EQ(parser.GetArgumentValue("files", 0), "file1.txt");
    ASSERT_EQ(parser.GetArgumentValue("files", 1), "file2.txt");
    ASSERT_EQ(parser.GetArgumentValue("files", 2), "file3.txt");
}

TEST_F(ArgParserTest, NargsZeroOrMoreTests_ImmediateZeroOrMoreNone) {
    Parser parser;
    parser.AddArgument("-f", "--files", "files", kImmediateZeroOrMore);
    parser.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    
    // ImmediateZeroOrMore без значений
    auto argv = CreateArgv({"-f", "-v"});
    parser.Parse(3, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("files"));
    ASSERT_EQ(parser.ArgumentSize("files"), 0);
    ASSERT_TRUE(parser.ArgumentInvolved("verbose"));
}

TEST_F(ArgParserTest, NargsZeroOrMoreTests_DelayedZeroOrMoreMultiple) {
    Parser parser;
    parser.AddArgument("-o", "--outputs", "outputs", kDelayedZeroOrMore);
    
    // Несколько значений для DelayedZeroOrMore
    auto argv = CreateArgv({"-o", "out1.txt", "out2.txt", "out3.txt"});
    parser.Parse(5, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("outputs"));
    ASSERT_EQ(parser.ArgumentSize("outputs"), 3);
    ASSERT_EQ(parser.GetArgumentValue("outputs", 0), "out1.txt");
    ASSERT_EQ(parser.GetArgumentValue("outputs", 1), "out2.txt");
    ASSERT_EQ(parser.GetArgumentValue("outputs", 2), "out3.txt");
}

TEST_F(ArgParserTest, NargsZeroOrMoreTests_DelayedZeroOrMoreNone) {
    Parser parser;
    parser.AddArgument("-o", "--outputs", "outputs", kDelayedZeroOrMore);
    parser.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    
    // DelayedZeroOrMore без значений
    auto argv = CreateArgv({"-o", "-v"});
    parser.Parse(3, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("outputs"));
    ASSERT_EQ(parser.ArgumentSize("outputs"), 0);
    ASSERT_TRUE(parser.ArgumentInvolved("verbose"));
}

TEST_F(ArgParserTest, NargsZeroOrMoreTests_MixedZeroOrMore) {
    Parser parser;
    parser.AddArgument("-f", "--files", "files", kImmediateZeroOrMore);
    parser.AddArgument("-o", "--outputs", "outputs", kDelayedZeroOrMore);
    
    // Смешивание Immediate и Delayed ZeroOrMore
    auto argv = CreateArgv({"-f", "file1.txt", "file2.txt", "-o", "out1.txt", "out2.txt"});
    parser.Parse(7, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("files"));
    ASSERT_EQ(parser.ArgumentSize("files"), 2);
    ASSERT_EQ(parser.GetArgumentValue("files", 0), "file1.txt");
    ASSERT_EQ(parser.GetArgumentValue("files", 1), "file2.txt");
    
    ASSERT_TRUE(parser.ArgumentInvolved("outputs"));
    ASSERT_EQ(parser.ArgumentSize("outputs"), 2);
    ASSERT_EQ(parser.GetArgumentValue("outputs", 0), "out1.txt");
    ASSERT_EQ(parser.GetArgumentValue("outputs", 1), "out2.txt");
}

// ============================================================================
// Группа 7: NargsSpecialTests - тестирование CommandOnly и RemainingArguments
// ============================================================================

TEST_F(ArgParserTest, NargsSpecialTests_CommandOnlyFlag) {
    Parser parser;
    parser.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    
    // CommandOnly как флаг
    auto argv = CreateArgv({"-v"});
    parser.Parse(2, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("verbose"));
    ASSERT_EQ(parser.ArgumentSize("verbose"), 0);
}

TEST_F(ArgParserTest, NargsSpecialTests_CommandOnlyMultiple) {
    Parser parser;
    parser.AddArgument("-a", "--alpha", "alpha", kCommandOnly);
    parser.AddArgument("-b", "--beta", "beta", kCommandOnly);
    parser.AddArgument("-c", "--gamma", "gamma", kCommandOnly);
    
    // Несколько CommandOnly аргументов
    auto argv = CreateArgv({"-a", "-b", "-c"});
    parser.Parse(4, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("alpha"));
    ASSERT_TRUE(parser.ArgumentInvolved("beta"));
    ASSERT_TRUE(parser.ArgumentInvolved("gamma"));
}

TEST_F(ArgParserTest, NargsSpecialTests_RemainingArgumentsSingle) {
    Parser parser;
    parser.AddArgument("", "", "remaining", kRemainingArguments);
    
    // RemainingArguments с одним аргументом
    auto argv = CreateArgv({"file.txt"});
    parser.Parse(2, argv.data());
    
    ASSERT_EQ(parser.ArgumentSize("remaining"), 1);
    ASSERT_EQ(parser.GetArgumentValue("remaining", 0), "file.txt");
}

TEST_F(ArgParserTest, NargsSpecialTests_RemainingArgumentsMultiple) {
    Parser parser;
    parser.AddArgument("", "", "remaining", kRemainingArguments);
    
    // RemainingArguments с несколькими аргументами
    auto argv = CreateArgv({"file1.txt", "file2.txt", "file3.txt"});
    parser.Parse(4, argv.data());
    
    ASSERT_EQ(parser.ArgumentSize("remaining"), 3);
    ASSERT_EQ(parser.GetArgumentValue("remaining", 0), "file1.txt");
    ASSERT_EQ(parser.GetArgumentValue("remaining", 1), "file2.txt");
    ASSERT_EQ(parser.GetArgumentValue("remaining", 2), "file3.txt");
}

TEST_F(ArgParserTest, NargsSpecialTests_RemainingArgumentsMixed) {
    Parser parser;
    parser.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    parser.AddArgument("-f", "--file", "file", kImmediateOptional);
    parser.AddArgument("", "", "remaining", kRemainingArguments);
    
    // RemainingArguments с другими типами аргументов
    auto argv = CreateArgv({"-v", "-f=input.txt", "extra1.txt", "extra2.txt"});
    parser.Parse(5, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("verbose"));
    ASSERT_TRUE(parser.ArgumentInvolved("file"));
    ASSERT_EQ(parser.GetArgumentValue("file", 0), "input.txt");
    ASSERT_EQ(parser.ArgumentSize("remaining"), 2);
    ASSERT_EQ(parser.GetArgumentValue("remaining", 0), "extra1.txt");
    ASSERT_EQ(parser.GetArgumentValue("remaining", 1), "extra2.txt");
}

// ============================================================================
// Группа 8: QueryMethodsTests - тестирование методов запросов
// ============================================================================

TEST_F(ArgParserTest, QueryMethodsTests_ArgumentInvolvedTrue) {
    Parser parser;
    parser.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    parser.AddArgument("-f", "--file", "file", kImmediateOptional);
    
    // Проверка ArgumentInvolved для использованного аргумента
    auto argv = CreateArgv({"-v", "-f=test.txt"});
    parser.Parse(3, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("verbose"));
    ASSERT_TRUE(parser.ArgumentInvolved("file"));
}

TEST_F(ArgParserTest, QueryMethodsTests_ArgumentInvolvedFalse) {
    Parser parser;
    parser.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    parser.AddArgument("-f", "--file", "file", kImmediateOptional);
    
    // Проверка ArgumentInvolved для неиспользованного аргумента
    auto argv = CreateArgv({"-v"});
    parser.Parse(2, argv.data());
    
    ASSERT_TRUE(parser.ArgumentInvolved("verbose"));
    ASSERT_FALSE(parser.ArgumentInvolved("file"));
}

TEST_F(ArgParserTest, QueryMethodsTests_ArgumentSize) {
    Parser parser;
    parser.AddArgument("-f", "--files", "files", kImmediateZeroOrMore);
    parser.AddArgument("-v", "--verbose", "verbose", kCommandOnly);
    
    // Проверка ArgumentSize для разных случаев
    auto argv = CreateArgv({"-f", "file1.txt", "file2.txt", "-v"});
    parser.Parse(5, argv.data());
    
    ASSERT_EQ(parser.ArgumentSize("files"), 2);
    ASSERT_EQ(parser.ArgumentSize("verbose"), 0);
    ASSERT_EQ(parser.ArgumentSize("nonexistent"), 0);
}

TEST_F(ArgParserTest, QueryMethodsTests_GetArgumentValueValid) {
    Parser parser;
    parser.AddArgument("-f", "--file", "file", kImmediateOptional);
    parser.AddArgument("-o", "--outputs", "outputs", kImmediateZeroOrMore);
    
    // Получение значения по валидному индексу
    auto argv = CreateArgv({"-f=test.txt", "-o", "out1.txt", "out2.txt"});
    parser.Parse(5, argv.data());
    
    ASSERT_EQ(parser.GetArgumentValue("file", 0), "test.txt");
    ASSERT_EQ(parser.GetArgumentValue("outputs", 0), "out1.txt");
    ASSERT_EQ(parser.GetArgumentValue("outputs", 1), "out2.txt");
}

TEST_F(ArgParserTest, QueryMethodsTests_GetArgumentValueExceptions) {
    Parser parser;
    parser.AddArgument("-f", "--file", "file", kImmediateOptional);
    
    auto argv = CreateArgv({"-f=test.txt"});
    parser.Parse(2, argv.data());
    
    // Проверка исключения при передаче nullptr
    EXPECT_THROW(parser.GetArgumentValue(nullptr, 0), Exception);
    
    // Проверка исключения при несуществующем аргументе
    EXPECT_THROW(parser.GetArgumentValue("nonexistent", 0), Exception);
    
    // Проверка ArgumentInvolved с nullptr
    ASSERT_FALSE(parser.ArgumentInvolved(nullptr));
    
    // Проверка ArgumentSize с nullptr
    ASSERT_EQ(parser.ArgumentSize(nullptr), 0);
}
