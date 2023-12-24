#pragma once
#include <fstream>
#include <memory>
#include "handler.h"

class Server
{
public:
    Server();
    bool handle(DTO& dto);
    void serverUp(int command);
    bool getCommandFlag() const;
    MYSQL* getMysql() const;
    void setDefaultStatuses();
private:
    enum Commands { SIGNUP = 1, SIGNIN, POST, READ, FIND, USERS, SIGNOUT };
    bool commandFlag_{ false };
    std::unique_ptr<Handler> handler_;
    void setConfigs();
    void setHandler(std::unique_ptr<Handler>&& handler);
    std::unique_ptr<MYSQL> mysql_;
    DTO conf_;
};
