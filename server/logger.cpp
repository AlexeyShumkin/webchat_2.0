#include <iostream>
#include "logger.h"

Logger::Logger()
{
    io_.open(path_, std::ios::in | std::ios::out | std::ios::app);
}

Logger::~Logger()
{
    io_.close();
}

void Logger::record(const std::string& line)
{
    io_.clear();
    auto tt = system_clock::to_time_t(system_clock::now());
    auto ptm = std::localtime(&tt);
    shmutex_.lock();
    io_ << std::put_time(ptm,"%F %X %Z") << ": " << line << std::endl;
    shmutex_.unlock();
}

void Logger::recite(int lineCount)
{
    io_.clear();
    io_.seekg(0);
    lineCount += 1;
    std::string lines[lineCount];
    int size = 0;
    shmutex_.lock_shared();
    while (io_.good())
    {
        std::getline(io_, lines[size % lineCount]);
        ++size;
    }
    shmutex_.unlock_shared();
    int start = size > lineCount ? (size % lineCount) : 0;
    int count = std::min(lineCount, size);
    for (int i = 0; i < count; i++)
    {
        auto index = (start + i) % lineCount;
        if (lines[index].empty())
            break;
        std::lock_guard<std::mutex> guard(lockCout_);
        std::cout << lines[index] << std::endl;
    }
}
