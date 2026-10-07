// bdc 0x08a29fa0 BtlBakuganIsCrystal
#include "bdc.h"

/* Returns 0 for the "is a crystal" battle-unit class test (virtual slot 11, `+0x5c`) in
   BtlBakugan's vtables. */

int BtlBakuganIsCrystal(BtlBakugan *self)
{
  (void)self;
  return 0;
}
