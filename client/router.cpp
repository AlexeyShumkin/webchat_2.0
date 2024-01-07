#include "router.h"

bool Router::establish()
{
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_file_descriptor == -1)
        return false;
    serveraddress.sin_addr.s_addr = inet_addr(addrBuf);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    return connection != -1;
}

const int Router::getSocketFD() const
{
    return socket_file_descriptor;
}

bool Router::pass(char command)
{
    bzero(buffer, BUF_SIZE);
    buffer[0] = command;
    write(socket_file_descriptor, buffer, sizeof(buffer));
    bzero(buffer, BUF_SIZE);
    read(socket_file_descriptor, buffer, sizeof(buffer));
    return buffer[0] & command;
}

bool Router::pass(const DTO& dto)
{
    for(const auto& data : dto)
    {
        auto size = std::to_string(data.size());
        bzero(buffer, BUF_SIZE);
        strcpy(buffer, size.c_str());
        write(socket_file_descriptor, buffer, sizeof(buffer));
        bzero(buffer, BUF_SIZE);
        read(socket_file_descriptor, buffer, sizeof(buffer));
        write(socket_file_descriptor, data.c_str(), data.size() + 1);
        bzero(buffer, BUF_SIZE);
        read(socket_file_descriptor, buffer, sizeof(buffer));
    }
    bzero(buffer, BUF_SIZE);
    strcpy(buffer, "end");
    write(socket_file_descriptor, buffer, sizeof(buffer));
    bzero(buffer, BUF_SIZE);
    read(socket_file_descriptor, buffer, sizeof(buffer));
    return buffer[0] == '1';
}

void Router::take(DTO& dto)
{
    dto.clear();
    while(true)
    {
        bzero(buffer, BUF_SIZE);
        auto bytes = read(socket_file_descriptor, buffer, sizeof(buffer));
        if(!strcmp("end", buffer) || bytes <= 0)
            break;
        auto digit = strlen(buffer);
        size_t size = 1;
        for(int i = 0; i < strlen(buffer); ++i)
            size += (buffer[i] - '0') * pow(10, --digit);
        bzero(buffer, BUF_SIZE);
        buffer[0] = '1';
        bytes = write(socket_file_descriptor, buffer, sizeof(buffer));
        if(bytes <= 0)
            break;
        char* tmp = new char[size];
        bytes = read(socket_file_descriptor, tmp, size);
        if(bytes <= 0)
            break;
        dto.push_back(tmp);
        delete[] tmp;
        bytes = write(socket_file_descriptor, buffer, sizeof(buffer));
        if(bytes <= 0)
            break;
    }
}

void Router::recordAddress(const std::string& address)
{
    bzero(addrBuf, 16);
    strcpy(addrBuf, address.c_str());
}
