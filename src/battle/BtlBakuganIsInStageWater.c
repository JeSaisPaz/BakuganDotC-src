// bdc 0x0885fcc8 BtlBakuganIsInStageWater
#include "bdc.h"

/* Returns 1 when the unit stands on floor material 4 and the current stage (script global 1,
   `g_scriptGlobalVars`) is one with real water (0..7, 0x10, 0x11). Gates splash effects and water
   behaviour. */
int BtlBakuganIsInStageWater(BtlBakugan *self)
{
    if (self->floorMaterial == 4) {
        switch ((u32)g_scriptGlobalVars[1]) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 0x10:
        case 0x11:
            return 1;
        default:
            break;
        }
    }
    return 0;
}
