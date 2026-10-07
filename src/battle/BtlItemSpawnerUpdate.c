// bdc 0x088a86d4 BtlItemSpawnerUpdate
#include "bdc.h"

/* State machine of an item spawner (`state`). State 0 waits for `g_btlItemSpawnEnabled`, then
   goes to 11 when the event path already spawned (`eventSpawned`) or to 50. Event path: 10 rolls
   `CoreRandNext`(100) — 70..99 or an item still lying there (`BtlItemSpawnerHasItem`) go to
   50, otherwise it sets `eventSpawned` and advances; 11 takes the first-spawn delay
   (`BtlItemSpawnerGetDelay`) into `timer` and spawns at once with it, 12 spawns with `timer`
   (`BtlItemSpawnerSpawnItem`); 13 counts `timer` down until it runs out or the item is gone;
   14 stops the item-point effects (`BtlItemSpawnerStopEffects`), clears `eventSpawned` and goes
   to 50. Regular cycle: 50 takes the respawn delay and falls into 51, which counts `timer` down
   and on expiry goes to 10 (52 also goes to 10). Other states do nothing. */
void BtlItemSpawnerUpdate(BtlItemSpawner *spawner)
{
    int delay;
    int hasItem;

    switch (spawner->state) {
    case 0:
        if (g_btlItemSpawnEnabled != 0) {
            spawner->state = spawner->eventSpawned != 0 ? 11 : 50;
        }
        break;
    case 10:
        if ((s32)CoreRandNext(100) >= 70) {
            spawner->state = 50;
        } else if (BtlItemSpawnerHasItem(spawner) != 0) {
            spawner->state = 50;
        } else {
            spawner->eventSpawned = 1;
            spawner->state++;
        }
        break;
    case 11:
        delay = BtlItemSpawnerGetDelay(spawner, 1);
        spawner->timer = delay;
        spawner->state++;
        BtlItemSpawnerSpawnItem(spawner, delay);
        spawner->state++;
        break;
    case 12:
        BtlItemSpawnerSpawnItem(spawner, spawner->timer);
        spawner->state++;
        break;
    case 13:
        hasItem = BtlItemSpawnerHasItem(spawner);
        spawner->timer--;
        if (spawner->timer < 0 || hasItem == 0) {
            spawner->state++;
        }
        break;
    case 14:
        BtlItemSpawnerStopEffects();
        spawner->eventSpawned = 0;
        spawner->state = 50;
        break;
    case 50:
        spawner->timer = BtlItemSpawnerGetDelay(spawner, 0);
        spawner->state++;
        /* fall through */
    case 51:
        spawner->timer--;
        if (spawner->timer < 0) {
            spawner->state++;
            spawner->state = 10;
        }
        break;
    case 52:
        spawner->state = 10;
        break;
    default:
        break;
    }
}
