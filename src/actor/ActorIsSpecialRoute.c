// bdc 0x08a2c438 ActorIsSpecialRoute
#include "bdc.h"

/* Base actor virtual slot 19 (`+0x9c`): returns 0; the guard classes override it with
   `ActorNpcGuardIsSpecialRoute`. */

int ActorIsSpecialRoute(Actor *self)

{
  return 0;
}

