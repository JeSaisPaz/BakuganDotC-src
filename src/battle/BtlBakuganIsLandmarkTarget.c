// bdc 0x08a29f00 BtlBakuganIsLandmarkTarget
#include "bdc.h"

/* Default class predicate of the battle-unit vtables (entry 19, fn at `+0x9c`): returns 0; only
   the landmark target point (`BtlTargetPointLandmarkIsLandmarkTarget`) answers 1. */
int BtlBakuganIsLandmarkTarget(BtlBakugan *self)
{
    (void)self;
    return 0;
}
