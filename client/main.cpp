#include "models.h"
#include "router.h"
#include "controllers.h"

int User::id = 0;

int main()
{
    ClientController cc;
    cc.run();
}