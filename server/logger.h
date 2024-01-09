#pragma once
#include <fstream>
#include <shared_mutex>
#include <mutex>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <string>

using std::chrono::system_clock;

class Logger
{
public:
    Logger();
    ~Logger();
    void record(const std::string& line);
    void recite(int lineCount);
private:
    std::string path_{ "log.txt" };
    std::fstream io_;
    std::shared_mutex shmutex_;
    std::mutex lockCout_;
};
