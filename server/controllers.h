#pragma once
#include "models.h"

class State;

class ServerController
{
public:
    ServerController();
    ~ServerController();
    void run();
    void exit();
private:
    std::unique_ptr<Server> server_;
    void respond(DTO& dto);
    std::unique_ptr<Router> router_;
};

class ClientController
{
public:
    ClientController();
    ~ClientController();
    void run();
    void request();
    bool send(DTO& dto, int command);
private:
    friend class State;
    bool active_{ true };
    std::unique_ptr<Server> server_;
    void setState(std::unique_ptr<State>& state);
    std::unique_ptr<State> state_;
};
