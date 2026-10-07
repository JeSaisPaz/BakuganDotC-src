// bdc 0x0889e1e8 BtlStageUpdateUvAnims
#include "bdc.h"

/* Per-frame step of the four arena UV animation slots registered by `BtlStageSetupUvAnim`: by
   slot type calls `BtlStageUvAnimStepType1` (type 1), `BtlStageUvAnimStepType2` (types 2 and 4)
   or `BtlStageUvAnimStepType3` (type 3) on the slot's parameter block; other types do nothing.
   Called by the battle/demo update loops (`BtlDemoUpdateObjects`). */
void BtlStageUpdateUvAnims(void)
{
    int slot;

    for (slot = 0; slot < 4; slot++) {
        switch (g_btlStageUvAnimTypes[slot]) {
        case 1:
            BtlStageUvAnimStepType1(g_btlStageUvAnimParams[slot]);
            break;
        case 2:
            BtlStageUvAnimStepType2(g_btlStageUvAnimParams[slot]);
            break;
        case 3:
            BtlStageUvAnimStepType3(g_btlStageUvAnimParams[slot]);
            break;
        case 4:
            BtlStageUvAnimStepType2(g_btlStageUvAnimParams[slot]);
            break;
        default:
            break;
        }
    }
}
