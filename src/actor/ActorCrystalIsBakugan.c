// bdc 0x08a29f78 ActorCrystalIsBakugan
#include "bdc.h"

/* Returns 0 for the "is a Bakugan" (BtlBakugan and its CPU subclasses) battle-unit class test
   (virtual slot 10, `+0x54`) in ActorCrystal's vtable. */

int ActorCrystalIsBakugan(void *unit)

{
  return 0;
}

