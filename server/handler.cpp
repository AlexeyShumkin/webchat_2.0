#include "handler.h"
#include "models.h"

size_t Handler::makeDialogID(const std::string& sender, const std::string& recipient)
{
    size_t i = 0;
    size_t res = 0;
    size_t sum = sender.size() + recipient.size();
    while(i < sender.size() && i < recipient.size())
    {
        res += sender[i] + recipient[i];
        ++i;
    }
    return res <<= sum;
}

size_t Handler::hashFunction(const std::string& password)
{
    size_t i = 0;
    size_t j = password.size() - 1;
    size_t res = 0;
    while(i < j)
        res += password[i++] << password[j--];
    return res;
}

bool SignUpHandler::specHandle(DTO& dto)
{
    if(!fs::exists(Server::userDataPath_ / dto[0]))
    {
        size_t hash = hashFunction(dto[1]);
        std::ofstream out;
        out.open(Server::userDataPath_ / dto[0]);
        if(out.is_open())
        {
            out << hash;
            out.close();
            return true;
        }
    }
    return false;
}

bool SignInHandler::specHandle(DTO& dto)
{
    if(fs::exists(Server::userDataPath_ / dto[0]))
    {
        size_t hash = hashFunction(dto[1]);
        size_t tmp = 0;
        std::ifstream in;
        in.open(Server::userDataPath_ / dto[0]);
        if(in.is_open())
        {
            in >> tmp;
            in.close();
        }
        return hash == tmp;
    }
    return false;
}

bool PostHandler::specHandle(DTO& dto)
{
    std::string room;
    if(dto[1] == "all")
        room = dto[1];
    else
        room = std::to_string(makeDialogID(dto[0], dto[1]));
    std::ofstream out;
    out.open(Server::msgDataPath_ / room, std::fstream::app);
    if(out.is_open())
    {
        out << dto[0] << " -> " << dto[1] << " [ " << dto[2] << " ] " << dto[3] << '\n';
        out.close();
        return true;
    }
    return false;
}

bool ReadHandler::specHandle(DTO& dto)
{
    std::string room;
    if(dto[1] == "all")
        room = dto[1];
    else
        room = std::to_string(makeDialogID(dto[0], dto[1]));
    if(!fs::exists(Server::msgDataPath_ / room))
        return false;
    std::ifstream in(Server::msgDataPath_ / room);
    dto.clear();
	for (std::string line; std::getline(in, line);) 
    	dto.push_back(line);
    return true;
}

bool FindUserHandler::specHandle(DTO& dto)
{
    return fs::exists(Server::userDataPath_ / dto[0]);
}

bool UserDisplayHandler::specHandle(DTO& dto)
{
    auto offName = Server::userDataPath_.c_str();
    fs::path path;
    dto.clear();
    for (const auto& entry : fs::directory_iterator(Server::userDataPath_))
    {
		path = entry;
		auto user = path.generic_string();
		user = user.substr(strlen(offName) + 1);
		dto.push_back(user);
	}
    return true;
}
