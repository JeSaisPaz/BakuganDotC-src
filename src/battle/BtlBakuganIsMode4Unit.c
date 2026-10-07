// bdc 0x08a29ef8 BtlBakuganIsMode4Unit
#include "bdc.h"

/* Default class predicate of the battle-unit vtables (entry 18, fn at `+0x94`): returns 0 — only
   the mode-4 unit (`BtlUnitMode4IsMode4Unit`) answers 1. */

int BtlBakuganIsMode4Unit(BtlBakugan *self)

{
  (void)self;
  return 0;
}

