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
        sign();
        if(cc->send(dto_, "1"))
            std::cout << "Registration was successful!\n";
        else
            std::cout << "This login is already taken!\n";
        break;
    case '2':
        sign();
        if(cc->send(dto_, "2"))
        {
            std::cout << "Welcome to the chat room!\n";
            setState(cc, std::make_unique<PubRoomControl>(PubRoomControl(dto_[0])));
        }
        else
            std::cout << "Invalid login or password!\n";
        break;
    case 'q':
        exit(cc);
        break;
    default:
        std::cout << "Your command is unclear. Please, select an action from the list:\n";
    }
}

void SignControl::sign()
{
    std::string login;
	std::string password;
	std::cout << "Enter your login: ";
	std::cin >> login;
	std::cout << "Enter your password: ";
	std::cin >> password;
    dto_.clear();
    dto_.push_back(login);
    dto_.push_back(password);
}

PubRoomControl::PubRoomControl(const std::string& sender)
{
    dto_.push_back(sender);
    dto_.push_back(recipient_);
}

void PubRoomControl::request(ClientController* cc)
{
    while(dto_.size() > 2)
        dto_.pop_back();
    char action = '0';
    std::cout << "Send message(1), read chat(2), dialog(3), display users(4), sign out(5), exit(q): ";
    std::cin >> action;
    switch(action)
    {
    case '1':
        if(!post())
            std::cout << "You can't send an empty message!\n";
        else
        {
            if(!cc->send(dto_, "3"))
                std::cout << "Failed dispatch!\n";
            else
                std::cout << "Successful dispatch!\n";
        }
        break;
    case '2':
        read(cc);
        break;
    case '3':
        if(setRecipient(cc))
            setState(cc, std::make_unique<PvtRoomControl>(PvtRoomControl(dto_[0], recipient_)));
        break;
    case '5':
        std::cout << "User " << dto_[0] << " left the chat room.\n";
        setState(cc, std::make_unique<SignControl>(SignControl()));
        break;
    case 'q':
        exit(cc);
        break;
    default:
        std::cout << "Your command is unclear. Please, select an action from the list:\n";
    }
}

bool PubRoomControl::post()
{
    std::string text;
	std::cout << "Message: ";
	std::getline(std::cin.ignore(), text);
    if(text.empty())
        return false;
    dto_.push_back(text);
    dto_.push_back(getCurrentTime());
    return true;
}

std::string PubRoomControl::getCurrentTime()
{
    time_t now = time(nullptr);
	char buffer[20];
	strftime(buffer, sizeof(buffer), "%X %d/%m/%Y", localtime(&now));
	return buffer;
}

void PubRoomControl::read(ClientController* cc)
{
    DTO dto{ recipient_ };
    std::string command{"4"};
    if(!cc->send(dto, command))
        std::cout << "There are no messages in the chat room yet!\n";
    else
    {
        for(const auto& data : dto)
            std::cout << data << '\n';
    }
}

bool PubRoomControl::setRecipient(ClientController* cc)
{
    std::string recipient;
    std::cout << "Enter the recipient login: ";
	std::cin >> recipient;
    if(recipient == dto_[0])
    {
        std::cout << "The developer still believes that users should not send messages to themselves :)\n";
        return false;
    }
    DTO dto{ recipient };
    if(!cc->send(dto, "5"))
    {
        std::cout << "There is no user with this login in the chat room!\n";
        return false;
    }
    else
        recipient_ = recipient;
    return true;
}

PvtRoomControl::PvtRoomControl(const std::string& sender, const std::string& recipient)
{
    dto_.push_back(sender);
    recipient_ = recipient;
    dto_.push_back(recipient_);
}

void PvtRoomControl::request(ClientController* cc)
{
    while(dto_.size() > 2)
        dto_.pop_back();
    char action = '0';
    std::cout << "Send message(1), read conversation(2), back chat(3), exit(q): ";
    std::cin >> action;
     switch(action)
    {
    case '1':
        break;
    case '2':
        break;
    case '3':
        setState(cc, std::make_unique<PubRoomControl>(PubRoomControl(dto_[0])));
        break;
    case 'q':
        exit(cc);
        break;
    default:
        std::cout << "Your command is unclear. Please, select an action from the list:\n";
    }
}
