// bdc 0x08a29ef0 BtlBakuganIsLandmarkAttrTarget
#include "bdc.h"

/* Default class predicate "is the target point of an attribute landmark" (unit vtable entry 16,
   offset `+0x84`) of the battle unit classes: returns false; only the landmark-attr TargetPoint
   class (`BtlTargetPointLandmarkAttrCtor`) overrides it. */
bool BtlBakuganIsLandmarkAttrTarget(BtlBakugan *self)
{
    (void)self;
    return false;
}
