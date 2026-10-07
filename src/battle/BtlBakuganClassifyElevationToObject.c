// bdc 0x08864128 BtlBakuganClassifyElevationToObject
#include "bdc.h"

/* Returns `BtlBakuganClassifyElevationToPoint` of `bakugan` against the world position
   (`GfxModel` `pos`) of the object `other`. Used by `BtlBakuganStartAttack` to choose
   level/high/low attack variants (1/5/4). */
int BtlBakuganClassifyElevationToObject(BtlBakugan *bakugan, void *other)
{
    return BtlBakuganClassifyElevationToPoint(bakugan, ((GfxModel *)other)->pos);
}
