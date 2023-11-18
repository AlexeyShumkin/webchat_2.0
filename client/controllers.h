#pragma once
#include "states.h"

class ClientController
{
public:
    ClientController();
    ~ClientController();
    void run();
    void request();
private:
    friend class State;
    void setState(std::unique_ptr<State>& state);
    bool active_{ true };
    std::unique_ptr<Router> router_;
    std::shared_ptr<State> state_;
    DTO dto_;
};