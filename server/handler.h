#pragma once
#include "router.h"

class Handler
{
public:
    virtual bool specHandle(DTO& dto) = 0;
protected:
    size_t makeDialogID(const std::string& sender, const std::string& recipient);
	size_t hashFunction(const std::string& password);
};

class SignUpHandler : public Handler
{
public:
    bool specHandle(DTO& dto) override;
};

class SignInHandler : public Handler
{
public:
    bool specHandle(DTO& dto) override;
};

class PostHandler : public Handler
{
public:
    bool specHandle(DTO& dto) override;
};

class ReadHandler : public Handler
{
public:
    bool specHandle(DTO& dto) override;
};

class FindUserHandler : public Handler
{
public:
    bool specHandle(DTO& dto) override;
};
