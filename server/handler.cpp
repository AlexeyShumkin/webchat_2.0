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

bool PubPostHandler::specHandle(DTO& dto)
{
    std::ofstream out;
    out.open(Server::msgDataPath_ / dto[1], std::fstream::app);
    if(out.is_open())
    {
        out << dto[0] << " -> all [ " << dto[2] << " ] " << dto[3] << '\n';
        out.close();
        return true;
    }
    return false;
}

bool PubReadHandler::specHandle(DTO& dto)
{
    if(!fs::exists(Server::msgDataPath_ / dto[0]))
        return false;
    std::ifstream in(Server::msgDataPath_ / dto[0]);
    dto.clear();
	for (std::string line; std::getline(in, line);) 
    	dto.push_back(line);
    return true;
}

bool FindUserHandler::specHandle(DTO& dto)
{
    return fs::exists(Server::userDataPath_ / dto[0]);
}
