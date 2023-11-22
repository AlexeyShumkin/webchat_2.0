#pragma once
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <string>
#include <iostream>

#define BUF_SIZE 1024
#define PORT 1666
using DTO = std::vector<std::string>;

class Router
{
public:
    bool establish();
    int getSocketFD() const;
    int getConnection() const;
    void passDTO(DTO& dto);
    void takeDTO(DTO& dto);
private:
    struct sockaddr_in serveraddress, client;
    socklen_t length;
    int socket_file_descriptor, connection, bind_status, connection_status;
    char buffer[BUF_SIZE];
};
