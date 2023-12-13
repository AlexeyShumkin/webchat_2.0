#include "controllers.h"

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

class SignControl : public State
{
public:
    void request(ClientController* cc) override;
    void sign();
    size_t hashFunction(const std::string& password);
};

class RoomControl : public State
{
public:
    RoomControl() = default;
    RoomControl(const std::string& sender);
    void request(ClientController* cc) override;
    bool post();
    std::string getCurrentTime();
    void read(ClientController* cc, int command);
    void setRecipient(ClientController* cc);
protected:
    std::string recipient_{"all"};
};
