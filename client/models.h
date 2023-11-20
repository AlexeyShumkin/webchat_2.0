#pragma once
#include "controllers.h"

class User
{
public:
    User() = default;
    User(const std::string& login, const std::string& password);
    int getID() const;
    static int id;
private:
    std::string login_;
    std::string password_;
    int id_; 
};

class Room
{
public:
    
protected:
    
};
