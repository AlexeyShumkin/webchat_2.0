#pragma once
#include "models.h"

class ServerController
{
public:
    ServerController();
    ~ServerController();
    void run();
    void signRespond();
    enum Commands { SIGNUP = 1, SIGNIN };
private:
    bool active_{ true };
    std::unique_ptr<Router> router_;
    std::unique_ptr<Server> server_;
    DTO dto_;
};