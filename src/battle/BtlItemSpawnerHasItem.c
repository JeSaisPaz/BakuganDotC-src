// bdc 0x088a8400 BtlItemSpawnerHasItem
#include "bdc.h"

/* Returns whether an item spawned by this spawner is still lying on the field
   (`BtlItemListHasFromSpawner`). */
int BtlItemSpawnerHasItem(void *spawner)
{
    return BtlItemListHasFromSpawner(spawner);
}
