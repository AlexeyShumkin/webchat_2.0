#pragma once
#include "models.h"

class State;

class ServerController
{
public:
    ServerController();
    ~ServerController();
    void run();
    void respond(DTO& dto, int command);
    void setHandler(int command);
    enum Commands { SIGNUP = 1, SIGNIN, POST, READ, FIND, USERS };
    static bool active_;
protected:
    std::unique_ptr<Server> server_;
private:
    std::unique_ptr<Router> router_;
};

class ClientController : public ServerController
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
    void setState(std::unique_ptr<State>& state);
    std::unique_ptr<State> state_;
};


