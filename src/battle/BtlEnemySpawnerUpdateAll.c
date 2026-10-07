// bdc 0x088a59d0 BtlEnemySpawnerUpdateAll
#include "bdc.h"

/* Runs `BtlEnemySpawnerUpdate` for every spawner in `g_btlEnemySpawnerList` (nothing when the
   list was never allocated). Called from the stage update `ActorStageObjRecordUpdateAll`. */

void BtlEnemySpawnerUpdateAll(void)
{
    CoreObject *spawner;

    if (g_btlEnemySpawnerList == NULL) {
        return;
    }
    for (spawner = g_btlEnemySpawnerList->head; spawner != NULL; spawner = spawner->next) {
        BtlEnemySpawnerUpdate(spawner);
    }
}
