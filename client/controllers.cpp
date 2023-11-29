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

bool ClientController::send(char command)
{
    return router_->pass(command);
}

bool ClientController::send(const DTO& dto)
{
    return router_->pass(dto);
}

void ClientController::take(DTO& dto)
{
    router_->take(dto);
}

void ClientController::setState(std::unique_ptr<State>& state)
{
    state_ = move(state);
}
