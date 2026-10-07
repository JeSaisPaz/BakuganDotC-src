// bdc 0x088a8920 BtlItemSpawnerUpdateAll
#include "bdc.h"

/* Ticks every item spawner in `g_btlItemSpawnerList` (`BtlItemSpawnerTick`; nothing when the
   list was never allocated). Called from the stage update `ActorStageObjRecordUpdateAll`. */

void BtlItemSpawnerUpdateAll(void)
{
    CoreObject *spawner;

    if (g_btlItemSpawnerList == NULL) {
        return;
    }
    for (spawner = g_btlItemSpawnerList->head; spawner != NULL; spawner = spawner->next) {
        BtlItemSpawnerTick(spawner);
    }
}
