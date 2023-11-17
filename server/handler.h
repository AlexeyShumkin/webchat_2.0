#pragma once
#include "router.h"

class Handler
{
public:
    virtual bool specHandle(DTO& dto) = 0;
protected:
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