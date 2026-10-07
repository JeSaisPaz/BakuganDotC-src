// bdc 0x0885911c ActorCrystalMode3ReturnFalse
#include "bdc.h"

/* Empty handler for crystal mode 3 (entry 3 of the mode member-pointer table run by
   ActorCrystalUpdateLogic): returns 0. */
int ActorCrystalMode3ReturnFalse(ActorCrystal *self)
{
    (void)self;
    return 0;
}
