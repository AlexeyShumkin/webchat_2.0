#pragma once
#include <fstream>
#include <shared_mutex>
#include <iomanip>
#include <ctime>
#include <chrono>

using std::chrono::system_clock;

class Logger
{
public:
    Logger();
    ~Logger();
    
    void record(const std::string& line);
    std::string recite();
private:
    std::string path_{ "log.txt" };
    std::fstream io_;
    std::shared_mutex shmutex_;
};
