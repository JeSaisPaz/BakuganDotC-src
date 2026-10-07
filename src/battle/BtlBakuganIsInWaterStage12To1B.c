// bdc 0x0885fd50 BtlBakuganIsInWaterStage12To1B
#include "bdc.h"

/* Returns 1 when the unit stands on floor material 4 (water) in one of the stages 0x12, 0x13,
   0x18, 0x19, 0x1a or 0x1b (script global variable 1, `g_scriptGlobalVars`); the third set of
   water stages next to `BtlBakuganIsInStageWater` and `BtlBakuganIsInWaterStage0CTo0F`. */
int BtlBakuganIsInWaterStage12To1B(BtlBakugan *self)
{
    if (self->floorMaterial == 4) {
        switch ((u32)g_scriptGlobalVars[1]) {
        case 0x12:
        case 0x13:
        case 0x18:
        case 0x19:
        case 0x1a:
        case 0x1b:
            return 1;
        }
    }
    return 0;
}
