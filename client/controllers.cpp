#include "controllers.h"

ClientController::ClientController()
{
    router_ = std::make_unique<Router>();
    state_ = std::make_unique<ConnectionControl>();
}

ClientController::~ClientController()
{
    close(router_->getSocketFD());
}

void ClientController::run()
{
    std::cout << "Hello, you can enter the server address or select a stored address to connect to this server.\n";
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
