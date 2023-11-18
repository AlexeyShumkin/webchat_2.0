#include "states.h"
#include "controllers.h"
#include "models.h"

void State::setState(ClientController* cc, std::unique_ptr<State>&& state)
{
    cc->setState(state);
}

const std::unique_ptr<Router>& State::getRouter(ClientController* cc) const
{
    return cc->router_;
}

DTO& State::getDTO(ClientController* cc)
{
    return cc->dto_;
}

void State::exit(ClientController* cc)
{
    cc->active_ = false;
}


void SignControl::request(ClientController* cc)
{
    char action = '0';
    std::cout << "Sign up(1), sign in(2), exit(q): ";
    std::cin >> action;
    switch(action)
    {
    case '1':
        if(send(cc, "1"))
        {
            std::cout << "Registration was successful!\n";
            // user_ = std::make_unique<User>(User(getDTO(cc)[0], getDTO(cc)[1]));
        }
        else
            std::cout << "This login is already taken!\n";
        break;
    case '2':
        if(send(cc, "2"))
        {
            std::cout << "Welcome to the chat room!\n";
        }
        else
            std::cout << "Invalid login or password!\n";
        break;
    }
}

bool SignControl::send(ClientController* cc, const std::string& command)
{
    DTO dto{ command };
    getRouter(cc)->passDTO(dto);
    getRouter(cc)->takeDTO(dto);
    if(!dto.empty())
    {
        sign(dto);
        getRouter(cc)->passDTO(dto);
        getRouter(cc)->takeDTO(dto);
        return !dto.empty();
    }
    return false;
}

void SignControl::sign(DTO& dto)
{
    std::cout << dto[0] << '\n';
    std::string login;
	std::string password;
	std::cout << "login: ";
	std::cin >> login;
	std::cout << "password: ";
	std::cin >> password;
    dto.clear();
    dto.push_back(login);
    dto.push_back(password);
}
