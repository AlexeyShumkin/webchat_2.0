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
    auto tt = system_clock::to_time_t(system_clock::now());
    auto ptm = std::localtime(&tt);
    if(!io_.is_open())
    {
        io_.open(path_, std::ios::out | std::ios::app);
    }
    shmutex_.lock();
    io_ << std::put_time(ptm,"%F %X %Z") << ": " << line << std::endl;
    shmutex_.unlock();
}

std::string Logger::recite()
{
    std::string line;
    if(!io_.is_open())
    {
        io_.open(path_, std::ios::in);
    }
    if(!io_.eof())
    {
        shmutex_.lock_shared();
        for(std::string tmp; std::getline(io_, tmp);)
        {
            line = move(tmp);
        }
        shmutex_.unlock_shared();
    }
    return line;
}
