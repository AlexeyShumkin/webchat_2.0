#include "states.h"

void State::setState(ClientController* cc, std::unique_ptr<State>&& state)
{
    cc->setState(state);
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
        if(cc->send(dto_, 1))
            std::cout << "Registration was successful!\n";
        else
            std::cout << "This login is already taken!\n";
        break;
    case '2':
        sign();
        if(cc->send(dto_, 2))
        {
            std::cout << "Welcome to the chat room!\n";
            setState(cc, std::make_unique<RoomControl>(RoomControl(dto_[0])));
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
    auto hash = std::to_string(hashFunction(password));
    dto_.clear();
    dto_.push_back(login);
    dto_.push_back(hash);
}

size_t SignControl::hashFunction(const std::string& password)
{
    size_t i = 0;
    size_t j = password.size() - 1;
    size_t res = 0;
    while(i < j)
        res += password[i++] << password[j--];
    return res;
}

RoomControl::RoomControl(const std::string& sender)
{
    dto_.push_back(sender);
    dto_.push_back(recipient_);
}

void RoomControl::request(ClientController* cc)
{
    while(dto_.size() > 2)
        dto_.pop_back();
    char action = '0';
    std::cout << "Send message(1), read conversation(2), change room(3), display users(4), sign out(5), exit(q): ";
    std::cin >> action;
    switch(action)
    {
    case '1':
        if(post())
        {
            if(!cc->send(dto_, 3))
                std::cout << "Failed dispatch!\n";
            else
                std::cout << "Successful dispatch!\n";
        }
        break;
    case '2':
        read(cc, 4);
        break;
    case '3':
        setRecipient(cc);
        if(recipient_ != "all")
            std::cout << "To send messages to the general chat, specify the recipient <all>\n";
        break;
    case '4':
        read(cc, 6);
        break;
    case '5':
        cc->send(dto_, 7);
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

bool RoomControl::post()
{
    std::string text;
	std::cout << "Message: ";
	std::getline(std::cin.ignore(), text);
    if(text.empty())
        std::cout << "You can't send an empty message!\n";
    dto_.push_back(text);
    dto_.push_back(getCurrentTime());
    return !text.empty();
}

std::string RoomControl::getCurrentTime()
{
    time_t now = time(nullptr);
	char buffer[20];
	strftime(buffer, sizeof(buffer), "%X %d/%m/%Y", localtime(&now));
	return buffer;
}

void RoomControl::read(ClientController* cc, int command)
{
    DTO dto;
    if(command == 4)
    {
        dto.push_back(dto_[0]);
        dto.push_back(recipient_);
        if(!cc->send(dto, command))
            std::cout << "There are no messages in this room yet!\n";
        else
        {
            for(const auto& data : dto)
                std::cout << data << '\n';
        }
    }
    else if(command == 6)
    {
        cc->send(dto, command);
        int number = 1;
	    std::cout << "Now in chat room:\n";
        for(const auto& data : dto)
            std::cout << number++ << ") " << data << '\n';
    }
}

void RoomControl::setRecipient(ClientController* cc)
{
    std::string recipient;
    std::cout << "Enter the recipient login: ";
	std::cin >> recipient;
    if(recipient == dto_[0])
    {
        std::cout << "The developer still believes that users should not send messages to themselves :)\n";
        return;
    }
    DTO dto{ recipient };
    if(!cc->send(dto, 5) && recipient != "all")
    {
        std::cout << "There is no user with this login in the chat room!\n";
        return;
    }
    recipient_ = recipient;
    dto_[1] = recipient_;
}
