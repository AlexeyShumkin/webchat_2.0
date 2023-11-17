#include "controllers.h"
using namespace std::string_literals;

ClientController::ClientController()
{
    router_ = std::make_unique<Router>(Router());
}

ClientController::~ClientController()
{
    close(router_->getSocketFD());
}

void ClientController::run()
{
    if(router_->establish())
        std::cout << "Connection was successful!\n";
    std::cout << "Hello! You are welcome to register, or you can enter the chat room if you are already registered.\n";
    while(active_)
    {
        char action = '0';
        std::cout << "Sign up(1), sign in(2), exit(q): ";
        std::cin >> action;
        switch(action)
        {
        case '1':
            if(request("1"))
            {
                std::cout << "Registration was successful!\n";
                user_ = std::make_unique<User>(User(dto_[0], dto_[1]));
            }
            else
                std::cout << "This login is already taken!\n";
            break;
        case '2':
            if(request("2"))
                std::cout << "Welcome to the chat room!\n";
            else
                std::cout << "Invalid login or password!\n";
            break;
        }
    }
}

bool ClientController::request(const std::string& command)
{
    dto_.clear();
    dto_.push_back(command);
    router_->passDTO(dto_);
    router_->takeDTO(dto_);
    for(auto& d : dto_)
        std::cout << d << '\n';
    if(!dto_.empty())
    {
        sign();
        router_->passDTO(dto_);
        router_->takeDTO(dto_);
        return !dto_.empty();
    }
    return false;
}

void ClientController::sign()
{
    std::string login;
	std::string password;
	std::cout << "login: ";
	std::cin >> login;
	std::cout << "password: ";
	std::cin >> password;
    dto_.clear();
    dto_.push_back(login);
    dto_.push_back(password);
}
