#pragma once
#include <fstream>
#include "router.h"

class ClientController;

class State
{
public:
    virtual ~State() = default;
    virtual void request(ClientController* cc) = 0;
protected:
    void setState(ClientController* cc, std::unique_ptr<State>&& state);
    void exit(ClientController* cc);
    DTO dto_;
};

class ConnectionControl : public State
{
public:
    void request(ClientController* cc) override;
    bool setAddress(ClientController* cc);
    bool selectAddress(ClientController* cc);
    bool passAddress(ClientController* cc, const std::string& address);
private:
    std::string addrPath{"addresses"};
};

class SignControl : public State
{
public:
    void request(ClientController* cc) override;
    void sign();
};

class RoomControl : public State
{
public:
    explicit RoomControl(const std::string& sender);
    void request(ClientController* cc) override;
    bool post();
    void read(ClientController* cc, char command);
    void setRecipient(ClientController* cc);
protected:
    std::string recipient_{"all"};
};
