#pragma once
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <cmath>

#define BUF_SIZE 5
#define PORT 1666
using DTO = std::vector<std::string>;

class Router
{
public:
    bool establish();
    const int getSocketFD() const;
    bool pass(char command);
    bool pass(const DTO& dto);
    void take(DTO& dto);
    void recordAddress(const std::string& address);
private:
    struct sockaddr_in serveraddress, client;
    int socket_file_descriptor, connection;
    char buffer[BUF_SIZE];
    char addrBuf[16];
};
