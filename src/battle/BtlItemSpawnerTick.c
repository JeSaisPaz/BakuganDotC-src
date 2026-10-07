// bdc 0x088a88c8 BtlItemSpawnerTick
#include "bdc.h"

/* Runs `BtlItemSpawnerUpdate` only in battle (`BtlCameraTaskExists`) and while
   `g_btlBattleOutcome` is 0 (battle still undecided). */

void BtlItemSpawnerTick(void *spawner)
{
    bool active = false;

    if (BtlCameraTaskExists() != 0 && g_btlBattleOutcome == 0) {
        active = true;
    }
    if (active) {
        BtlItemSpawnerUpdate(spawner);
    }
}
