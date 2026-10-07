// bdc 0x08865114 BtlBakuganPlayLandingSound
#include "bdc.h"

/* Plays the landing sound for the floor material under the unit (`floorMaterial`): kind sound slot
   1 by default, 2 for material 3, 3 for water (material 4) when the stage has real water
   (`BtlBakuganIsInStageWater`); material 4 without stage water keeps slot 1. Played through
   `BtlBakuganPlayKindSound``(self, slot, 0, 0)`. */
void BtlBakuganPlayLandingSound(BtlBakugan *self)
{
    s32 material = self->floorMaterial;
    s32 slot = 1;

    if (material < 4) {
        if (material >= 3) {
            slot = 2;
        }
    } else if (material < 5 && BtlBakuganIsInStageWater(self) != 0) {
        slot = 3;
    }
    BtlBakuganPlayKindSound(self, slot, 0, 0);
}
