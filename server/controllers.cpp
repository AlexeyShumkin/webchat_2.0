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
    mysql_close(&server_->mysql);
}

void ServerController::run()
{
    router_->establish();
    while(router_->getStatus())
    {
        router_->wiretap();
        while(true)
        {
            DTO dto;
            int command = router_->take();
            if(command < 1)
                break;
            serverUp(command);
            respond(dto);
        }
    }
}

void ServerController::respond(DTO& dto)
{
    router_->take(dto);
    if(server_->handle(dto))
    {
        router_->pass('1');
        if(commandFlag_)
            router_->pass(dto);
    }
    else
        router_->pass('0');
}

void ServerController::serverUp(int command)
{
    if(commandFlag_)
        commandFlag_ = false;
    switch(command)
    {
    case SIGNUP:
        server_->setHandler(std::make_unique<SignUpHandler>(SignUpHandler()));
        break;
    case SIGNIN:
        server_->setHandler(std::make_unique<SignInHandler>(SignInHandler()));
        break;
    case POST:
        server_->setHandler(std::make_unique<PostHandler>(PostHandler()));
        break;
    case READ:
        server_->setHandler(std::make_unique<ReadHandler>(ReadHandler()));
        commandFlag_ = true;
        break;
    case FIND:
        server_->setHandler(std::make_unique<FindUserHandler>(FindUserHandler()));
        break;
    case USERS:
        server_->setHandler(std::make_unique<UserDisplayHandler>(UserDisplayHandler()));
        commandFlag_ = true;
    case SIGNOUT:
        server_->setHandler(std::make_unique<SignOutHandler>(SignOutHandler()));
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
    mysql_close(&server_->mysql);
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
    serverUp(command);
    return server_->handle(dto);
}

void ClientController::setState(std::unique_ptr<State>& state)
{
    state_ = move(state);
}
