#include "controllers.h"

ClientController::ClientController()
{
    router_ = std::make_unique<Router>(Router());
    state_ = std::make_unique<SignControl>(SignControl());
}

ClientController::~ClientController()
{
    close(router_->getSocketFD());
}

void ClientController::run()
{
    if(router_->establish())
        std::cout << "Hello! You are welcome to register, or you can enter the chat room if you are already registered.\n";
    while(active_)
        request();
    std::cout << "Goodbye!\n";
}

void ClientController::request()
{   
    state_->request(this);
}

void ClientController::setState(std::unique_ptr<State>& state)
{
    state_ = move(state);
}
