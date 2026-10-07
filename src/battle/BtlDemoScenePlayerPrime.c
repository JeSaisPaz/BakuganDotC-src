// bdc 0x088ff2e0 BtlDemoScenePlayerPrime
#include "bdc.h"

/* Hands a battle demo scene player task (task id 0x66, `BtlDemoScenePlayerCtor`) to its demo:
   stores the owning demo `demo` in `player->demo` and runs the player's update (vtable entry 2,
   `BtlDemoScenePlayerUpdate`) three times, re-reading the vtable each time, which steps it
   through `BtlDemoScenePlayerStateStart`, `BtlDemoScenePlayerStateLoad` (loads the `.scb`) and
   the first frame of `BtlDemoScenePlayerStatePlay`. Does nothing for a NULL `player`. */
void BtlDemoScenePlayerPrime(CoreTask *demo, BtlDemoScenePlayer *player)
{
    const VtblEntry *update;
    s32 i;

    if (player == NULL) {
        return;
    }
    player->demo = demo;
    for (i = 0; i < 3; i++) {
        update = &((const VtblEntry *)player->base.vtable)[2];
        ((void (*)(void *))update->fn)((u8 *)player + update->delta);
    }
}
