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
    while(active_)
    {
        router_->takeDTO(dto_);
        if(dto_.size() == 1)
        {
            switch(stoi(dto_[0]))
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
            }
            respond();
        }
    }
    std::cout << "Session end.\n";
}

void ServerController::respond()
{
    router_->passDTO(dto_);
    router_->takeDTO(dto_);
    if(server_->handle(dto_))
    {
        router_->passDTO(dto_);
        dto_.clear();
    }    
    else
    {
        dto_.clear();
        router_->passDTO(dto_);
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
    switch(command)
    {
    case ClientController::SIGNUP:
        server_->setHandler(std::make_unique<SignUpHandler>(SignUpHandler()));
        break;
    case ClientController::SIGNIN:
        server_->setHandler(std::make_unique<SignInHandler>(SignInHandler()));
        break;
    case ClientController::POST:
        server_->setHandler(std::make_unique<PostHandler>(PostHandler()));
        break;
    case ClientController::READ:
        server_->setHandler(std::make_unique<ReadHandler>(ReadHandler()));
        break;
    case ClientController::FIND:
        server_->setHandler(std::make_unique<FindUserHandler>(FindUserHandler()));
        break;
    case ClientController::USERS:
        server_->setHandler(std::make_unique<UserDisplayHandler>(UserDisplayHandler()));
        break;
    }
    return server_->handle(dto);
}

void ClientController::setState(std::unique_ptr<State>& state)
{
    state_ = move(state);
}

