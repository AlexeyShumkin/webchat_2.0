#include "controllers.h"

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
    if(router_->establish())
        std::cout << "Connection was successful!\n";
    while(active_)
    {
        router_->takeDTO(dto_);
        if(dto_.size() == 1)
        {
            switch(stoi(dto_[0]))
            {
            case ServerController::SIGNUP:
                server_->setHandler(std::make_unique<SignUpHandler>(SignUpHandler()));
                respond();
                break;
            case ServerController::SIGNIN:
                server_->setHandler(std::make_unique<SignInHandler>(SignInHandler()));
                respond();
                break;
            case ServerController::POST:
                server_->setHandler(std::make_unique<PubPostHandler>(PubPostHandler()));
                respond();
                break;
            case ServerController::READ:
                server_->setHandler(std::make_unique<PubReadHandler>(PubReadHandler()));
                respond();
            case ServerController::FIND:
                server_->setHandler(std::make_unique<FindUserHandler>(FindUserHandler()));
                respond();
            }
        }
        else
        {
            std::cout << "Session end.\n";
            break;
        }
    }
}

void ServerController::respond()
{
    router_->passDTO(dto_);
    router_->takeDTO(dto_);
    if(server_->handle(dto_))
        router_->passDTO(dto_);
    else
    {
        dto_.clear();
        router_->passDTO(dto_);
    }
}

