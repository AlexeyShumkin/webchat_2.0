#pragma once
#include "models.h"
#include "router.h"

class ClientController
{
public:
    ClientController();
    ~ClientController();
    void run();
    bool request(const std::string& command);
    void sign();
private:
    bool active_{ true };
    std::unique_ptr<User> user_;
    std::unique_ptr<Router> router_;
    DTO dto_;
};