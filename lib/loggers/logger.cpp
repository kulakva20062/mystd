#include "logger.hpp"
#include "../file_manager/dir_file.hpp"
#include "../file_manager/formats/file_log.hpp"
#include <filesystem>
#include <string>
#include <chrono>
#include <algorithm>

namespace fs = std::filesystem;

Logger::Logger(const fs::path& file_path)
    :file_txt_(file_path)
{
    file_txt_.Clear();
    file_txt_ += std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + "\n";
}

void Logger::Log(std::string_view message, size_t level)
{
    file_txt_ += std::string(message.begin(), message.end()) + " " + std::to_string(level) + "\n";
}

//TODO возможно стоит добавить какуе-то пометку для важных логов с ошибками в name файла или в самом файле

void Logger::CleanLogs(size_t max_files, size_t level_clean) {
    DirFile dir_file(file_txt_.GetParentPath());
    dir_file.Open();
    std::vector<std::pair<long long, FileLog*>> files;
    for (auto& file : dir_file.Data()) {
        //Считываем, как txt файл
        auto file_log = dynamic_cast<FileLog*>(file.get());
        if (!file_log) {
            file->DeleteFile();
            continue;
        }
        //Все строки(линии) файла
        std::vector<std::string> lines = std::move(file_log->GetLines());
        if (lines.size() == 0) {
            file_log->DeleteFile();
            continue;
        }
        //Время создания файла
        long long time = atoll(lines[0].c_str());
        if (time == 0) {
            file_log->DeleteFile();
            continue;
        }
        //Обрабатываем level каждой строки(линии), если он больше level, то файл остаётся в любом случае, иначе добовляется в вектор files.
        bool is_delete = true;
        for (auto it = lines.begin() + 1; it != lines.end(); ++it) {
            auto& line = *it;
            auto words = std::move(LineToWords(line));
            if (words.size() == 0 || (atoll(words[words.size() - 1].c_str()) == 0 && words[words.size() - 1] != "0")) {
                file_log->DeleteFile();
                break;
            }
            if (atoll(words[words.size() - 1].c_str()) > level_clean) {
                is_delete = false;
                break;
            }
            
        }
        if (is_delete) {
            files.push_back(std::make_pair(time, file_log));
        }
    }
    //Удаление самых старых логов, если их больше max_files
    std::sort(files.begin(), files.end(),
    [](const std::pair<long long, FileTxt*>& a, const std::pair<long long, FileTxt*>& b)->bool{
        return a.first > b.first;
    });
    if (files.size() > max_files) {
        for (size_t index = max_files; index < files.size(); ++index) {
            files[index].second->DeleteFile();
        }
    }
    dir_file.Close();
}
