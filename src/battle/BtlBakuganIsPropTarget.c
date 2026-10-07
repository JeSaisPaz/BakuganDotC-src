// bdc 0x08a29ee8 BtlBakuganIsPropTarget
#include "bdc.h"

/* Default class predicate "is the target point of a scenery prop" (unit vtable entry 15, offset
   `+0x7c`) of the battle unit classes: returns false; only the prop TargetPoint class
   (`BtlTargetPointPropCtor`) overrides it to return true. */
bool BtlBakuganIsPropTarget(BtlBakugan *self)
{
    (void)self;
    return false;
}
