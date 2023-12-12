#pragma once
#include "router.h"
#include <mysql/mysql.h>

class Handler
{
public:
    virtual ~Handler() = default;
    virtual bool specHandle(DTO& dto, MYSQL* mysql) = 0;
protected:
    size_t makeDialogID(const std::string& sender, const std::string& recipient);
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
};

class ReadHandler : public Handler
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
