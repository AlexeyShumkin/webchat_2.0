#include "controllers.h"

int main()
{
   // ServerController sc;
   // sc.run();
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
