#include "controllers.h"
#include "states.h"

ServerController::ServerController()
{
    server_ = std::make_unique<Server>();
    router_ = std::make_unique<Router>();
}

ServerController::~ServerController()
{
    close(router_->getSocketFD());
    mysql_close(server_->getMysql());
    std::string esc{"kill "};
    esc += std::to_string(pid_);
    system(esc.c_str());
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
            {
                break;
            }
            server_->serverUp(command);
            respond(dto);
        }
    }
}

void ServerController::exit()
{
    router_->turnOff();
}

const int ServerController::getPid() const
{
    return pid_;
}

void ServerController::setPid(int pid)
{
    pid_ = pid;
}

void ServerController::respond(DTO& dto)
{
    router_->take(dto);
    if(server_->handle(dto))
    {
        router_->pass('1');
        if(server_->getCommandFlag())
            router_->pass(dto);
    }
    else
        router_->pass('0');
}

ClientController::ClientController()
{
    server_ = std::make_unique<Server>();
    state_ = std::make_unique<SignControl>();
}

ClientController::~ClientController()
{
    server_->setDefaultStatuses();
    mysql_close(server_->getMysql());
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
    server_->serverUp(command);
    return server_->handle(dto);
}

void ClientController::viewLog(int lineCount)
{
    server_->viewLog(lineCount);
}

void ClientController::setState(std::unique_ptr<State>& state)
{
    state_ = move(state);
}
