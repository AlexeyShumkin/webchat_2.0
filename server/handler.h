#pragma once
#include "router.h"
#include <mysql/mysql.h>

class Handler
{
public:
    virtual ~Handler() = default;
    virtual bool specHandle(DTO& dto, MYSQL* mysql) = 0;
protected:
    MYSQL_RES* res;
	MYSQL_ROW row;
};

class SignUpHandler : public Handler
{
public:
    bool specHandle(DTO& dto, MYSQL* mysql) override;
};

class SignInHandler : public Handler
{
public:
    bool specHandle(DTO& dto, MYSQL* mysql) override;
};

class PostHandler : public Handler
{
public:
    bool specHandle(DTO& dto, MYSQL* mysql) override;
protected:
    std::hash<std::string> hasher;
    std::string hash(const std::string& sender, const std::string& recipient);
    int getRoomID(DTO& dto, MYSQL* mysql);
};

class ReadHandler : public PostHandler
{
public:
    bool specHandle(DTO& dto, MYSQL* mysql) override;
};

class FindUserHandler : public Handler
{
public:
    bool specHandle(DTO& dto, MYSQL* mysql) override;
};

class UserDisplayHandler : public Handler
{
public:
    bool specHandle(DTO& dto, MYSQL* mysql) override;
};

class SignOutHandler : public Handler
{
public:
    bool specHandle(DTO& dto, MYSQL* mysql) override;
};
