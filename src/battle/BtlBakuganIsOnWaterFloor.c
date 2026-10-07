// bdc 0x0885fcb8 BtlBakuganIsOnWaterFloor
#include "bdc.h"

/* Returns whether the floor material under the unit (set by `BtlBakuganProbeGround`) is
   4 (water). */
int BtlBakuganIsOnWaterFloor(BtlBakugan *bakugan)
{
    return bakugan->floorMaterial == 4;
}
