#include "controllers.h"

int main()
{
   ServerController sc;
   sc.setPid(fork());
   if(sc.getPid())
   {
      sc.run();
   }  
   else
   {
      ClientController cc;
      cc.run();
   }
   sc.exit();
}
