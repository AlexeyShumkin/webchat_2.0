#pragma once
#include <string>
#include <memory>
#include <iostream>

class User
{
public:
    User(const std::string& login, const std::string& password);
    int getID() const;
    static int id;
private:
    friend std::ostream& operator << (std::ostream& os, const User& user);
    std::string login_;
    std::string password_;
    int id_; 
};

class Room
{
public:
    
protected:
    
};
