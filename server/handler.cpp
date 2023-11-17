#include "handler.h"
#include "models.h"

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
