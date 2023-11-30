#include <unistd.h>
#include "controllers.h"

fs::path Server::userDataPath_{ "data/userData" };
fs::path Server::msgDataPath_{ "data/msgData" };

int main()
{
   int pid = fork();
   if(pid)
   {
      ServerController sc;
      sc.run();
   }  
   else if(!pid)
   {
      ClientController cc;
      cc.run();
   }
}
