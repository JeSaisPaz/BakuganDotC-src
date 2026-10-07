// bdc 0x0889fd88 GameGimmickCorePointState01Collected
#include "bdc.h"

/* State 1 of the core-point gimmick (table `0x08a83c54`, after pickup): fades the draw alpha
   `+0x6c` by 0.2 per frame; after 20 frames (`+0x188`) stops the pickup effect 0xe, sets the timer
   to 150 and advances to state 2. */

void GameGimmickCorePointState01Collected(GameGimmickCorePoint *obj)
{
    float *alpha = &obj->base.base.ambient[3];

    *alpha = *alpha - 0.2f;
    if (obj->base.base.ambient[3] < 0.0f) {
        obj->base.base.ambient[3] = 0.0f;
    }
    obj->timer = obj->timer + 1;
    if (obj->timer > 20) {
        GfxEffectStopAttached(g_worldEffectMgr, 0xe, &obj->base.base.data->rootMatrix[12]);
        obj->timer = 0x96;
        obj->cpState = obj->cpState + 1;
    }
}
