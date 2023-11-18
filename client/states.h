#pragma once
#include "router.h"

class ClientController;
class User;

class State
{
public:
    virtual ~State() = default;
    virtual void request(ClientController* cc) = 0;
    virtual bool send(ClientController* cc, const std::string& command) = 0;
protected:
    void setState(ClientController* cc, std::unique_ptr<State>&& state);
    const std::unique_ptr<Router>& getRouter(ClientController* cc) const;
    DTO& getDTO(ClientController* cc);
    void exit(ClientController* cc);
};

class SignControl : public State
{
public:
    void request(ClientController* cc) override;
    bool send(ClientController* cc, const std::string& command) override;
    void sign(DTO& dto);
private:
    // std::unique_ptr<User> user_;
};