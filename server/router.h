#pragma once
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>

#define BUF_SIZE 5
#define PORT 1666
using DTO = std::vector<std::string>;

class Router
{
public:
    bool establish();
    int getSocketFD() const;
    int take();
    void take(DTO& dto);
    void pass(char answer);
    void pass(const DTO& dto);
    int getClientAddr() const;
private:
    struct sockaddr_in serveraddress, client;
    socklen_t length;
    int socket_file_descriptor, connection, bind_status, connection_status;
    char buffer[BUF_SIZE];
};
