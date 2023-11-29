#include "controllers.h"
#include "states.h"

ServerController::ServerController()
{
    server_ = std::make_unique<Server>(Server());
    router_ = std::make_unique<Router>(Router());
}

ServerController::~ServerController()
{
    close(router_->getSocketFD());
}

void ServerController::run()
{
    router_->establish();
    DTO dto;
    while(active_)
    {
        int command = router_->take();
        setHandler(command);
        respond(dto, command);
    }
    std::cout << "Session end.\n";
}

void ServerController::respond(DTO& dto, int command)
{
    router_->take(dto);
    if(server_->handle(dto))
    {
        router_->pass('1');
        if(command == 4 || command == 6)
            router_->pass(dto);
    }
    else
        router_->pass('0');
}

void ServerController::setHandler(int command)
{
    switch(command)
    {
    case ServerController::SIGNUP:
        server_->setHandler(std::make_unique<SignUpHandler>(SignUpHandler()));
        break;
    case ServerController::SIGNIN:
        server_->setHandler(std::make_unique<SignInHandler>(SignInHandler()));
        break;
    case ServerController::POST:
        server_->setHandler(std::make_unique<PostHandler>(PostHandler()));
        break;
    case ServerController::READ:
        server_->setHandler(std::make_unique<ReadHandler>(ReadHandler()));
        break;
    case ServerController::FIND:
        server_->setHandler(std::make_unique<FindUserHandler>(FindUserHandler()));
        break;
    case ServerController::USERS:
        server_->setHandler(std::make_unique<UserDisplayHandler>(UserDisplayHandler()));
        break;
    }
}

ClientController::ClientController()
{
    server_ = std::make_unique<Server>(Server());
    state_ = std::make_unique<SignControl>(SignControl());
}

ClientController::~ClientController()
{
    ServerController::active_ = false;
}

void ClientController::run()
{
    std::cout << "Hello! You are welcome to register, or you can enter the chat room if you are already registered.\n";
    while(active_)
        request();
    std::cout << "Goodbye!\n";
}

void ClientController::request()
{
    state_->request(this);
}

bool ClientController::send(DTO& dto, int command)
{
    setHandler(command);
    return server_->handle(dto);
}

void ClientController::setState(std::unique_ptr<State>& state)
{
    state_ = move(state);
}
