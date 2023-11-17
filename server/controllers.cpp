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
                signRespond();
                break;
            case ServerController::SIGNIN:
                server_->setHandler(std::make_unique<SignInHandler>(SignInHandler()));
                signRespond();
                break;
            }
        }
        else
        {
            std::cout << "Session end.\n";
            break;
        }
    }
}

void ServerController::signRespond()
{
    dto_.clear();
    dto_.push_back("Enter your data");
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

