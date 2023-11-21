#pragma once
#include <fstream>
#include <filesystem>
#include <memory>
#include "handler.h"

namespace fs = std::filesystem;
class ServerController;
class ClientController;

class Server
{
public:
    Server();
    bool handle(DTO& dto);
    static fs::path userDataPath_;
	static fs::path msgDataPath_;
private:
    friend class ServerController;
    friend class ClientController;
    std::unique_ptr<Handler> handler_;
    void setHandler(std::unique_ptr<Handler>&& handler);
};