#include "handler.h"
#include "models.h"

bool SignUpHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    std::string query = "insert into users(login, pw_hash) values('" + dto[0] + "','" + dto[1] + "')";
    return mysql_query(mysql, query.c_str()) == 0;
}

bool SignInHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    std::string query = "select exists(select id from users where login ='" + dto[0] + "' and pw_hash ='" + dto[1] + "')";
    mysql_query(mysql, query.c_str());
    res = mysql_store_result(mysql);
    row = mysql_fetch_row(res);
    if(*row[0] == '1')
    {
        query = "update users set status = 'online' where login ='" + dto[0] + "'";
        return mysql_query(mysql, query.c_str()) == 0;
    }
    return false;
}

bool PostHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    std::string room;
    if (dto[1] == "all")
        room = "general";
    else
        room = (hash(dto[0], dto[1]));
    int id = getRoomID(mysql, room);
    std::string query;
    if(id <= 0)
    {
        query = "insert into rooms(title) values('" + room + "')";
        mysql_query(mysql, query.c_str());
        id = getRoomID(mysql, room);
    }
    query = "call post(" + std::to_string(id) + ",'" + dto[0] + "','" + dto[1] + "','" + dto[2] + "')";
    return mysql_query(mysql, query.c_str()) == 0;
}

std::string PostHandler::hash(const std::string& sender, const std::string& recipient)
{
    auto hash1 = hasher(sender);
    auto hash2 = hasher(recipient);
    return std::to_string(hash1 ^ hash2);
}

int PostHandler::getRoomID(MYSQL* mysql, const std::string& room)
{
    std::string query = "select exists(select id from rooms where title ='" + room + "')";
    mysql_query(mysql, query.c_str());
    res = mysql_store_result(mysql);
    row = mysql_fetch_row(res);
    if (*row[0] == '1')
    {
        query = "select id from rooms where title ='" + room + "'";
        mysql_query(mysql, query.c_str());
        res = mysql_store_result(mysql);
        row = mysql_fetch_row(res);
    }
    int id = atoi(row[0]);
    return id;
}

bool ReadHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    std::string room;
    if (dto[1] == "all")
        room = "general";
    else
        room = (hash(dto[0], dto[1]));
    int id = getRoomID(mysql, room);
    if(id > 0)
    {
        std::string query = "select sender,recipient,content,received_at from msgdata where room_id = " + std::to_string(id);
        mysql_query(mysql, query.c_str());
        res = mysql_store_result(mysql);
        dto.clear();
        while(row = mysql_fetch_row(res))
        {   
            std::string tmp;
            for(size_t i = 0, counter = 1; i < mysql_num_fields(res); ++i, ++counter)
            {
                tmp += row[i];
                if(counter == 1)
                    tmp += " -> ";
                else if(counter == 2)
                    tmp += " [ ";
                else if(counter == 3)
                    tmp += " ] ";
                else if(counter == 4)
                {
                    dto.push_back(tmp);
                    counter = 0;
                    tmp.clear();
                }
            }
        }
        return true;
    }
    return false;
}

bool FindUserHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    std::string query = "select exists(select id from users where login ='" + dto[0] + "')";
    mysql_query(mysql, query.c_str());
    res = mysql_store_result(mysql);
    row = mysql_fetch_row(res);
    return *row[0] == '1';
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
