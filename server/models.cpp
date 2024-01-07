#include "models.h"

Server::Server()
{
    logger_ = std::make_unique<Logger>();
    mysql_ = std::make_unique<MYSQL>();
    mysql_init(mysql_.get());
    setConfigs();
	if(!mysql_real_connect(mysql_.get(), conf_[0].c_str(), conf_[1].c_str(), conf_[2].c_str(), conf_[3].c_str(), 
    stoi(conf_[4]), nullptr, 0))
         std::cout << "Connection error: " << mysql_error(mysql_.get()) << std::endl;
}

void Server::setConfigs()
{
    std::ifstream in("db_conf");
	for (std::string line; std::getline(in, line);) 
        conf_.push_back(line);
}

bool Server::handle(DTO& dto)
{
    if(dto.size() > 2)
    {
        std::string line = dto[0] + " -> " + dto[1] + " [ " + dto[2] + " ]";
        logger_->record(line);
    }
    return handler_->specHandle(dto, mysql_.get());
}

void Server::serverUp(int command)
{
    if(commandFlag_)
        commandFlag_ = false;
    switch(command)
    {
    case SIGNUP:
        setHandler(std::make_unique<SignUpHandler>());
        break;
    case SIGNIN:
        setHandler(std::make_unique<SignInHandler>());
        break;
    case POST:
        setHandler(std::make_unique<PostHandler>());
        break;
    case READ:
        setHandler(std::make_unique<ReadHandler>());
        commandFlag_ = true;
        break;
    case FIND:
        setHandler(std::make_unique<FindUserHandler>());
        break;
    case USERS:
        setHandler(std::make_unique<UserDisplayHandler>());
        commandFlag_ = true;
        break;
    case SIGNOUT:
        setHandler(std::make_unique<SignOutHandler>());
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

void Server::setDefaultStatuses()
{
    const char* query = "update users set status = default";
    mysql_query(mysql_.get(), query);
}

void Server::setHandler(std::unique_ptr<Handler>&& handler)
{
    handler_ = move(handler);
}
