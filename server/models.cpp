#include "models.h"

Server::Server()
{
    mysql_ = std::make_unique<MYSQL>(MYSQL());
    mysql_init(mysql_.get());
    setConfigs();
	mysql_real_connect(mysql_.get(), conf_[0].c_str(), conf_[1].c_str(), conf_[2].c_str(), conf_[3].c_str(), 
    stoi(conf_[4]), nullptr, 0);
    const char* query = "call complete";
    mysql_query(mysql_.get(), query);
}

void Server::setConfigs()
{
    std::ifstream in("db_conf");
	for (std::string line; std::getline(in, line);) 
        conf_.push_back(line);
}

bool Server::handle(DTO& dto)
{
    return handler_->specHandle(dto, mysql_.get());
}

void Server::serverUp(int command)
{
    if(commandFlag_)
        commandFlag_ = false;
    switch(command)
    {
    case SIGNUP:
        setHandler(std::make_unique<SignUpHandler>(SignUpHandler()));
        break;
    case SIGNIN:
        setHandler(std::make_unique<SignInHandler>(SignInHandler()));
        break;
    case POST:
        setHandler(std::make_unique<PostHandler>(PostHandler()));
        break;
    case READ:
        setHandler(std::make_unique<ReadHandler>(ReadHandler()));
        commandFlag_ = true;
        break;
    case FIND:
        setHandler(std::make_unique<FindUserHandler>(FindUserHandler()));
        break;
    case USERS:
        setHandler(std::make_unique<UserDisplayHandler>(UserDisplayHandler()));
        commandFlag_ = true;
        break;
    case SIGNOUT:
        setHandler(std::make_unique<SignOutHandler>(SignOutHandler()));
        break;
    }
}

bool Server::getCommandFlag() const
{
    return commandFlag_;
}

MYSQL* Server::getMysql() const
{
    return mysql_.get();
}

void Server::setHandler(std::unique_ptr<Handler>&& handler)
{
    handler_ = move(handler);
}
