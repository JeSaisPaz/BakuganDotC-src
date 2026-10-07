// bdc 0x088a85e4 BtlItemSpawnerSpawnItem
#include "bdc.h"

/* Spawns one item at a copy of the spawner's layout-record position: rolls the kind with
   BtlItemRollKind from the current stage's item odds row 4 (pctB = kind 1/2 share, pctA = kind 3,
   pctC = kind 0, no 'none' share) and, unless the roll is 'none' (6), creates the item with
   lifetime `life`, flag 1 and this spawner, then spawns the appear marker. */
void BtlItemSpawnerSpawnItem(BtlItemSpawner *spawner, int life)
{
    float pos[4];
    BtlItemOdds *odds;
    int kind;

    pos[0] = spawner->rec->pos[0];
    pos[1] = spawner->rec->pos[1];
    pos[2] = spawner->rec->pos[2];
    pos[3] = spawner->rec->pos[3];
    odds = (BtlItemOdds *)BtlStageGetItemOdds(4);
    kind = BtlItemRollKind(odds->pctB, odds->pctA, odds->pctC, 0);
    if (kind != 6) {
        BtlItemCreate(kind, (u32 *)pos, life, 1, spawner);
        BtlItemSpawnerSpawnMarker(spawner);
    }
}
