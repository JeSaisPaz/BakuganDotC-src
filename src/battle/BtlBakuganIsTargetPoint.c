// bdc 0x08a29ee0 BtlBakuganIsTargetPoint
#include "bdc.h"

/* Default class predicate "is a target point" (unit vtable entry 14, offset `+0x74`) of the battle
   unit classes: returns false; `BtlTargetPoint` and its subclasses override it
   to return true (`0x08a29fd0`). */

bool BtlBakuganIsTargetPoint(BtlBakugan *self)

{
  (void)self;
  return false;
}

