// bdc 0x088a54f0 BtlEnemySpawnerCtor
#include "bdc.h"

/* Constructor of the `BtlEnemySpawner` (`CoreObject`-derived, 0x40 bytes, vtable
   `g_btlEnemySpawnerVtbl`): clears `id`, `state`, `timer`, `enabled` and `spawnCount`, sets the
   spawn centre to (0, -400, 4500, 0) and appends itself to `g_btlEnemySpawnerList`. The list
   pointer is read before `BtlEnemySpawnerEnsureList` creates a missing list, so the very first
   spawner is appended to NULL (as in the binary). Returns `spawner`. */
BtlEnemySpawner *BtlEnemySpawnerCtor(BtlEnemySpawner *spawner)
{
    CoreObjectList *list;

    CoreObjectInit(&spawner->base, NULL);
    spawner->base.vtable = g_btlEnemySpawnerVtbl;
    spawner->id = 0;
    spawner->state = 0;
    spawner->centerX = 0.0f;
    spawner->centerY = -400.0f;
    spawner->centerZ = 4500.0f;
    spawner->centerW = 0.0f;
    spawner->timer = 0;
    spawner->enabled = 0;
    spawner->spawnCount = 0;
    list = g_btlEnemySpawnerList;
    if (g_btlEnemySpawnerList == NULL) {
        BtlEnemySpawnerEnsureList();
    }
    CoreObjectListAppend(&spawner->base, list);
    return spawner;
}
