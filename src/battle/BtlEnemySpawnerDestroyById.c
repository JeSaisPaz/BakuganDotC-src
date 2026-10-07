// bdc 0x088a5acc BtlEnemySpawnerDestroyById
#include "bdc.h"

/* Destroys every enemy spawner in `g_btlEnemySpawnerList` whose `id` equals `id`, through its
   virtual destructor (vtable entry 1) with flags 3. The next link is read before the call, so the
   walk survives the spawner unlinking itself. Does nothing while the list holder does not exist.
   Called by `ScriptOpEnemySpawner` (mode 0, `on <= 0`); counterpart of `BtlEnemySpawnerCreate`. */
void BtlEnemySpawnerDestroyById(int id)
{
    BtlEnemySpawner *spawner;
    BtlEnemySpawner *next;

    if (g_btlEnemySpawnerList == NULL) {
        return;
    }
    for (spawner = (BtlEnemySpawner *)g_btlEnemySpawnerList->head; spawner != NULL; spawner = next) {
        next = (BtlEnemySpawner *)spawner->base.next;
        if (spawner->id == id) {
            const VtblEntry *dtor = &((const VtblEntry *)spawner->base.vtable)[1];

            ((void (*)(void *, s32))dtor->fn)((u8 *)spawner + dtor->delta, 3);
        }
    }
}
