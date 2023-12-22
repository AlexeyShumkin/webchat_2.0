#include "router.h"

void Router::establish()
{
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_file_descriptor == -1)
    {
        std::cout << "Socket creation failed!\n";
        return;
    }
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    bind_status = bind(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if(bind_status == -1)  
    {
        std::cout << "Socket binding failed!\n";
        return;
    }
    status_ = true;
}

int Router::getSocketFD() const
{
    return socket_file_descriptor;
}

void Router::wiretap()
{
    if(!status_)
        return;
    connection_status = listen(socket_file_descriptor, 5);
    if(connection_status == -1)
    {
        std::cout << "Socket is unable to listen for new connections!\n";
        return;
    }  
    length = sizeof(client);
    connection = accept(socket_file_descriptor,(struct sockaddr*)&client, &length);
    if(connection == -1) 
    {
        std::cout << "Server is unable to accept the data from client!\n";
        return;
    }
}

int Router::take()
{
    bzero(buffer, BUF_SIZE);
    read(connection, buffer, sizeof(buffer));
    write(connection, buffer, sizeof(buffer));
    return buffer[0] - '0';
}

void Router::take(DTO& dto)
{
    dto.clear();
    while(true)
    {
        bzero(buffer, BUF_SIZE);
        read(connection, buffer, sizeof(buffer));
        if(!strcmp("end", buffer))
            break;
        auto digit = strlen(buffer);
        size_t size = 1;
        for(int i = 0; i < strlen(buffer); ++i)
            size += (buffer[i] - '0') * pow(10, --digit);
        bzero(buffer, BUF_SIZE);
        buffer[0] = '1';
        write(connection, buffer, sizeof(buffer));
        char* tmp = new char[size];
        read(connection, tmp, size);
        dto.push_back(tmp);
        delete[] tmp;
        write(connection, buffer, sizeof(buffer));
    }
}

void Router::pass(char answer)
{
    bzero(buffer, BUF_SIZE);
    buffer[0] = answer;
    write(connection, buffer, sizeof(buffer));
}

void Router::pass(const DTO& dto)
{
    for(const auto& data : dto)
    {
        auto size = std::to_string(data.size());
        bzero(buffer, BUF_SIZE);
        strcpy(buffer, size.c_str());
        write(connection, buffer, sizeof(buffer));
        bzero(buffer, BUF_SIZE);
        read(connection, buffer, sizeof(buffer));
        write(connection, data.c_str(), data.size() + 1);
        bzero(buffer, BUF_SIZE);
        read(connection, buffer, sizeof(buffer));
    }
    bzero(buffer, BUF_SIZE);
    strcpy(buffer, "end");
    write(connection, buffer, sizeof(buffer));
}

bool Router::getStatus() const
{
    return status_;
}

void Router::turnOff()
{
    status_ = false;
}
