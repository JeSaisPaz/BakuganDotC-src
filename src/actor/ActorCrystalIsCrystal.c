// bdc 0x08a29f80 ActorCrystalIsCrystal
#include "bdc.h"

/* Returns 1 for the "is a crystal" battle-unit class test (virtual slot 11, `+0x5c`) in
   ActorCrystal's vtable. */

int ActorCrystalIsCrystal(void *unit)

{
  return 1;
}

