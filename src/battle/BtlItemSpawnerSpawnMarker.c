// bdc 0x088a85b8 BtlItemSpawnerSpawnMarker
#include "bdc.h"

/* Spawns the item-appear effect 0x60 at the position of the spawner's layout record (`rec->pos`)
   on the unit effect manager `g_btlUnitEffectMgr` (`GfxEffectSpawn`); called by
   `BtlItemSpawnerSpawnItem` after it creates an item. */
void BtlItemSpawnerSpawnMarker(BtlItemSpawner *self)
{
    GfxEffectSpawn(g_btlUnitEffectMgr, 0x60, self->rec->pos);
}
