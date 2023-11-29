#include "states.h"
#include "controllers.h"

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
        if(!cc->send(action))
            break;
        sign();
        if(cc->send(dto_))
            std::cout << "Registration was successful!\n";
        else
            std::cout << "This login is already taken!\n";
        break;
    case '2':
        if(!cc->send(action))
            break;
        sign();
        if(cc->send(dto_))
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
    dto_.clear();
    dto_.push_back(login);
    dto_.push_back(password);
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
            if(!cc->send('3'))
                break;
            if(!cc->send(dto_))
                std::cout << "Failed dispatch!\n";
            else
                std::cout << "Successful dispatch!\n";
        }
        break;
    case '2':
        read(cc, "4");
        break;
    case '3':
        setRecipient(cc);
        if(recipient_ != "all")
            std::cout << "To send messages to the general chat, specify the recipient <all>\n";
        break;
    case '4':
        read(cc, "6");
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

bool RoomControl::post()
{
    std::string text;
	std::cout << "Message: ";
	std::getline(std::cin.ignore(), text);
    if(text.empty())
    {
        std::cout << "You can't send an empty message!\n";
        return false;
    }    
    dto_.push_back(text);
    dto_.push_back(getCurrentTime());
    return true;
}

std::string RoomControl::getCurrentTime()
{
    time_t now = time(nullptr);
	char buffer[20];
	strftime(buffer, sizeof(buffer), "%X %d/%m/%Y", localtime(&now));
	return buffer;
}

void RoomControl::read(ClientController* cc, const std::string& command)
{
    DTO dto;
    if(command == "4")
    {
        dto.push_back(dto_[0]);
        dto.push_back(recipient_);
        if(1)
            std::cout << "There are no messages in this room yet!\n";
        else
        {
            for(const auto& data : dto)
                std::cout << data << '\n';
        }
    }
    else if(command == "6")
    {
        
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
    if(recipient != "all")
    {
        std::cout << "There is no user with this login in the chat room!\n";
        return;
    }
    recipient_ = recipient;
    dto_[1] = recipient_;
}
