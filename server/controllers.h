#pragma once
#include "models.h"

class State;

class ClientController
{
public:
    ClientController();
    ~ClientController();
    void run();
    void request();
    bool send(DTO& dto, int command);
    enum Commands { SIGNUP = 1, SIGNIN, POST, READ, FIND, USERS };
private:
    friend class State;
    bool active_{ true };
    void setState(std::unique_ptr<State>& state);
    std::unique_ptr<State> state_;
    std::unique_ptr<Server> server_;
};

class ServerController
{
public:
    ServerController();
    ~ServerController();
    void run();
    void respond(DTO& dto);
    enum Commands { SIGNUP = 1, SIGNIN, POST, READ, FIND, USERS };
    static bool active_;
private:
    std::unique_ptr<Router> router_;
    std::unique_ptr<Server> server_;
};
