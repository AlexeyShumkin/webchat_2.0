#include <unistd.h>
#include "controllers.h"

fs::path Server::userDataPath_{ "data/userData" };
fs::path Server::msgDataPath_{ "data/msgData" };
bool ServerController::active_{ true };

int main()
{
   int pid = fork();
   if(pid > 0)
   {
      ServerController sc;
      sc.run();
   }
   else if(pid == 0)
   {
      ClientController cc;
      cc.run();
   }
}

