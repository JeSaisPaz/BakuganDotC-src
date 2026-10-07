// bdc 0x08855608 ActorCrystalDefaultType
#include "bdc.h"

/* Returns the constant 2: the default "type" argument for `ActorCrystalSetStyle` (`self` is
   passed by the callers but ignored). */
u32 ActorCrystalDefaultType(ActorCrystal *self)
{
    (void)self;
    return 2;
}
