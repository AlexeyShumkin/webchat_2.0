#pragma once
#include "models.h"

class State;

class ServerController
{
public:
    ServerController();
    ~ServerController();
    virtual void run();
    void serverUp(int command);
    enum Commands { SIGNUP = 1, SIGNIN, POST, READ, FIND, USERS };
protected:
    std::unique_ptr<Server> server_;
    bool commandFlag_{ false };
private:
    void respond(DTO& dto);
    std::unique_ptr<Router> router_;
};

class ClientController : public ServerController
{
public:
    ClientController();
    ~ClientController();
    void run() override;
    void request();
    bool send(DTO& dto, int command);
private:
    friend class State;
    bool active_{ true };
    void setState(std::unique_ptr<State>& state);
    std::unique_ptr<State> state_;
};


