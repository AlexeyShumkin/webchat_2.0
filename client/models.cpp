#include "models.h"

User::User(const std::string& login, const std::string& password) : login_{ login }, password_{ password }
{
    id_ = ++id;
}

int User::getID() const
{
    return id_;
}

std::ostream& operator<<(std::ostream& os, const User& user)
{
    return os << user.login_ << ' ' << user.password_ << std::endl;
}
