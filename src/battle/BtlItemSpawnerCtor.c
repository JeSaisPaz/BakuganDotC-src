// bdc 0x088a8290 BtlItemSpawnerCtor
#include "bdc.h"

/* Constructor of the battle item spawner (`BtlItemSpawner`, `CoreObject`-derived, vtable
   `g_btlItemSpawnerVtbl`, built by `ActorStageObjRecordSpawn` for item-point layout records):
   initialises the base with no list, stores the layout record `rec`, clears `state`, `field24`,
   `timer` and `eventSpawned`, and gives it the running index `index` from
   `g_btlItemSpawnerCount` (post-increment). Returns `spawner`. */
void *BtlItemSpawnerCtor(void *spawner, void *rec)
{
    BtlItemSpawner *self = (BtlItemSpawner *)spawner;
    s32 index;

    CoreObjectInit(&self->base, NULL);
    self->base.vtable = g_btlItemSpawnerVtbl;
    self->rec = rec;
    self->state = 0;
    self->field24 = 0;
    self->timer = 0;
    self->eventSpawned = 0;
    index = g_btlItemSpawnerCount;
    g_btlItemSpawnerCount = index + 1;
    self->index = index;
    return spawner;
}
