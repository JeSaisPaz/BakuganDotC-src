// bdc 0x0885ffec BtlBakuganHasAttackCommand
#include "bdc.h"

/* Returns whether the attack command bit 0x10 is set in the unit's command word `+0x16c` (pad/AI
   command flags built by `BtlInputReadActions`). */

int BtlBakuganHasAttackCommand(BtlBakugan *bakugan)

{
  return (bakugan->commands & 0x10) != 0;
}

