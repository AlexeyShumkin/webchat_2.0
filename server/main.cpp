#include "controllers.h"

fs::path Server::userDataPath_{ "data/userData" };
fs::path Server::msgDataPath_{ "data/msgData" };

int main()
{
   ServerController sc;
   sc.run();
}

