#include "router.h"

bool Router::establish()
{
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_file_descriptor == -1)
    {
        std::cout << "Socket creation failed!\n";
        return false;
    }
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    bind_status = bind(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if(bind_status == -1)  
    {
        std::cout << "Socket binding failed!\n";
        return false;
    }
    connection_status = listen(socket_file_descriptor, 5);
    if(connection_status == -1)
    {
        std::cout << "Socket is unable to listen for new connections!\n";
        return false;
    }  
    length = sizeof(client);
    connection = accept(socket_file_descriptor,(struct sockaddr*)&client, &length);
    if(connection == -1) 
    {
        std::cout << "Server is unable to accept the data from client!\n";
        return false;
    }
    return true;
}

int Router::getSocketFD() const
{
    return socket_file_descriptor;
}

int Router::getConnection() const
{
    return connection;
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
    ssize_t bytes = write(connection, buffer, sizeof(buffer));
}

void Router::takeDTO(DTO& dto)
{
    dto.clear();
    bzero(buffer, BUF_SIZE);
    read(connection, buffer, sizeof(buffer));
    for(int i = 0; i < strlen(buffer); ++i)
    {
        std::string tmp;
        while(buffer[i] != '|')
            tmp.push_back(buffer[i++]);
        dto.push_back(tmp);
    }
}
