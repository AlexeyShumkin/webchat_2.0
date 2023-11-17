#include "router.h"

bool Router::establish()
{
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_file_descriptor == -1)
    {
        std::cout << "Creation of Socket failed!\n";
        return false;
    }
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if(connection == -1)
    {
        std::cout << "Connection with the server failed!\n";
        return false;
    }
    return true;
}

const int Router::getSocketFD() const
{
    return socket_file_descriptor;
}

void Router::passDTO(DTO& dto)
{
    bzero(buffer, BUF_SIZE);
    int position = 0;
    for(auto& data : dto)
    {
        data += '|';
        strcpy(buffer + position, data.c_str());
        position += data.size();
    }
    ssize_t bytes = write(socket_file_descriptor, buffer, sizeof(buffer));
}

void Router::takeDTO(DTO& dto)
{   
    dto.clear();
    bzero(buffer, BUF_SIZE);
    read(socket_file_descriptor, buffer, sizeof(buffer));
    for(int i = 0; i < strlen(buffer); ++i)
    {
        std::string tmp;
        while(buffer[i] != '|')
            tmp.push_back(buffer[i++]);
        dto.push_back(tmp);
    }
}

