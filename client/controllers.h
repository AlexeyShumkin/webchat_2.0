#pragma once
#include "states.h"

class ClientController
{
public:
    ClientController();
    ~ClientController();
    void run();
    void request();
    bool send(char command);
    bool send(const DTO& dto);
private:
    friend class State;
    void setState(std::unique_ptr<State>& state);
    bool active_{ true };
    std::unique_ptr<Router> router_;
    std::unique_ptr<State> state_;
};
