#include "handler.h"
#include "models.h"

bool SignUpHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    query_ = "insert into users(login, pw_hash) values('" + dto[0] + "','" + dto[1] + "')";
    return mysql_query(mysql, query_.c_str()) == 0;
}

bool SignInHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    query_ = "select exists(select id from users where login ='" + dto[0] + "' and pw_hash ='" + dto[1] + "')";
    mysql_query(mysql, query_.c_str());
    res_ = mysql_store_result(mysql);
    row_ = mysql_fetch_row(res_);
    if(*row_[0] == '1')
    {
        query_ = "update users set status = 'online' where login ='" + dto[0] + "'";
        return mysql_query(mysql, query_.c_str()) == 0;
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
    if(id <= 0)
    {
        query_ = "insert into rooms(title) values('" + room + "')";
        mysql_query(mysql, query_.c_str());
        id = getRoomID(mysql, room);
    }
    query_ = "call post(" + std::to_string(id) + ",'" + dto[0] + "','" + dto[1] + "','" + dto[2] + "')";
    return mysql_query(mysql, query_.c_str()) == 0;
}

std::string Handler::hash(const std::string& sender, const std::string& recipient)
{
    auto hash1 = hasher_(sender);
    auto hash2 = hasher_(recipient);
    return std::to_string(hash1 ^ hash2);
}

int Handler::getRoomID(MYSQL* mysql, const std::string& room)
{
    query_ = "select exists(select id from rooms where title ='" + room + "')";
    mysql_query(mysql, query_.c_str());
    res_ = mysql_store_result(mysql);
    row_ = mysql_fetch_row(res_);
    if (*row_[0] == '1')
    {
        query_ = "select id from rooms where title ='" + room + "'";
        mysql_query(mysql, query_.c_str());
        res_ = mysql_store_result(mysql);
        row_ = mysql_fetch_row(res_);
    }
    int id = atoi(row_[0]);
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
        query_ = "select sender,recipient,content,received_at from msgdata where room_id = " + std::to_string(id);
        mysql_query(mysql, query_.c_str());
        res_ = mysql_store_result(mysql);
        dto.clear();
        while(row_ = mysql_fetch_row(res_))
        {   
            std::string tmp;
            for(size_t i = 0, counter = 1; i < mysql_num_fields(res_); ++i, ++counter)
            {
                tmp += row_[i];
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
    query_ = "select exists(select id from users where login ='" + dto[0] + "')";
    mysql_query(mysql, query_.c_str());
    res_ = mysql_store_result(mysql);
    row_ = mysql_fetch_row(res_);
    return *row_[0] == '1';
}

bool UserDisplayHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    std::string query_ = "select login, status from users";
    mysql_query(mysql, query_.c_str());
    res_ = mysql_store_result(mysql);
    dto.clear();
    while (row_ = mysql_fetch_row(res_))
    {
        std::string tmp;
        for (size_t i = 0, counter = 1; i < mysql_num_fields(res_); ++i, ++counter)
        {
            tmp += row_[i];
            if (counter == 1)
                tmp += '\t';
            else if (counter == 2)
            {
                dto.push_back(tmp);
                counter = 0;
                tmp.clear();
            }
        }
    }
    return true;
}

bool SignOutHandler::specHandle(DTO& dto, MYSQL* mysql)
{
    query_ = "update users set status = 'offline' where login ='" + dto[0] + "'";
    return mysql_query(mysql, query_.c_str()) == 0;
}
