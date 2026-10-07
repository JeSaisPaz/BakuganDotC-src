// bdc 0x0885fd14 BtlBakuganIsInWaterStage0CTo0F
#include "bdc.h"

/* Returns 1 when the unit stands on floor material 4 (water) in a stage whose id (script global
   variable 1, `g_scriptGlobalVars`) is 0xc..0xf; the counterpart of `BtlBakuganIsInStageWater`
   for the other set of water stages. */
int BtlBakuganIsInWaterStage0CTo0F(BtlBakugan *bakugan)
{
    if (bakugan->floorMaterial == 4 && g_scriptGlobalVars[1] > 0xb && g_scriptGlobalVars[1] < 0x10) {
        return 1;
    }
    return 0;
}
