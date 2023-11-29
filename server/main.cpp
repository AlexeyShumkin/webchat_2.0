#include <unistd.h>
#include "controllers.h"

fs::path Server::userDataPath_{ "data/userData" };
fs::path Server::msgDataPath_{ "data/msgData" };
bool ServerController::active_{ true };

int main()
{
   ServerController sc;
   // sc.run();
   int pid = fork();
   if(pid)
      sc.run();
   else if(!pid)
   {
      ClientController cc;
      cc.run();
   }
}
