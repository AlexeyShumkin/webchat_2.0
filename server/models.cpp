#include "models.h"

Server::Server()
{
    mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "root", "root", "servdb", 3306, nullptr, 0);
}

bool Server::handle(DTO& dto)
{
    return handler_->specHandle(dto, &mysql);
}

void Server::setHandler(std::unique_ptr<Handler>&& handler)
{
    handler_ = move(handler);
}
