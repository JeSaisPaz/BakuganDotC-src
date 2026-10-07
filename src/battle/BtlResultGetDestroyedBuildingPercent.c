// bdc 0x0883f510 BtlResultGetDestroyedBuildingPercent
#include "bdc.h"

/* Result-screen score item 0x1a: the percentage of the stage's buildings destroyed,
   `(int)((1 - intactRatio) * 100)` truncated towards zero, with `intactRatio` from
   `ActorStageObjGetIntactBuildingRatio`. `BtlResultGetScoreItem` multiplies it by 10 for the
   penalty item 0x1c. `hud` is unused. */
int BtlResultGetDestroyedBuildingPercent(void *hud)
{
    float intactRatio;

    (void)hud;
    intactRatio = ActorStageObjGetIntactBuildingRatio();
    return (int)((1.0f - intactRatio) * 100.0f);
}
