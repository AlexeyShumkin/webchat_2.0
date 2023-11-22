#include "models.h"

Server::Server()
{
    fs::create_directory("data");
	fs::create_directory(userDataPath_);
	fs::create_directory(msgDataPath_);
}

bool Server::handle(DTO& dto)
{
    return handler_->specHandle(dto);
}

void Server::setHandler(std::unique_ptr<Handler>&& handler)
{
    handler_ = move(handler);
}
