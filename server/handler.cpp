#include "handler.h"
#include "models.h"

size_t Handler::makeDialogID(const std::string& sender, const std::string& recipient)
{
    size_t i = 0;
    size_t res = 0;
    size_t sum = sender.size() + recipient.size();
    while(i < sender.size() && i < recipient.size())
    {
        res += sender[i] + recipient[i];
        ++i;
    }
    return res <<= sum;
}

bool SignUpHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    std::string query = "insert into users(login, hash) values('" + dto[0] + "','" + dto[1] + "')";
    return mysql_query(mysql, query.c_str()) == 0;
}

bool SignInHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    std::string query = "select exists(select id from users where login ='" + dto[0] + "' and hash ='" + dto[1] + "')";
    mysql_query(mysql, query.c_str());
    res = mysql_store_result(mysql);
    row = mysql_fetch_row(res);
    if(*row[0] == '1')
    {
        query = "update users set status = 'online' where login ='" + dto[0] + "'";
        return mysql_query(mysql, query.c_str()) == 0;
    }
    else
        return false;
}

bool PostHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    std::string room;
    if(dto[1] == "all")
        room = dto[1];
    else
        room = std::to_string(makeDialogID(dto[0], dto[1]));
    std::ofstream out;
    out.open(Server::msgDataPath_ / room, std::fstream::app);
    if(out.is_open())
    {
        out << dto[0] << " -> " << dto[1] << " [ " << dto[2] << " ] " << dto[3] << '\n';
        out.close();
        return true;
    }
    return false;
}

bool ReadHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    std::string room;
    if(dto[1] == "all")
        room = dto[1];
    else
        room = std::to_string(makeDialogID(dto[0], dto[1]));
    if(!fs::exists(Server::msgDataPath_ / room))
        return false;
    std::ifstream in(Server::msgDataPath_ / room);
    dto.clear();
	for (std::string line; std::getline(in, line);) 
    	dto.push_back(line);
    return true;
}

bool FindUserHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    return fs::exists(Server::userDataPath_ / dto[0]);
}

bool UserDisplayHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    auto offName = Server::userDataPath_.c_str();
    fs::path path;
    dto.clear();
    for (const auto& entry : fs::directory_iterator(Server::userDataPath_))
    {
		path = entry;
		auto user = path.generic_string();
		user = user.substr(strlen(offName) + 1);
		dto.push_back(user);
	}
    return true;
}

bool SignOutHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    std::string query = "update users set status = 'offline' where login ='" + dto[0] + "'";
    return mysql_query(mysql, query.c_str()) == 0;
}
