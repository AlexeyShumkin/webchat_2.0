#include "controllers.h"

int main()
{
   ServerController sc;
   int pid = fork();
   if(pid)
   {
      sc.run();
   }  
   else if(!pid)
   {
      ClientController cc;
      cc.run();
   }
   sc.exit();
}
