// bdc 0x089057c0 BtlDemoScenePlayerDtor
#include "bdc.h"

/* Destructor of the battle demo scene player task: does nothing for NULL; otherwise restores
   g_btlDemoScenePlayerVtbl, deletes every object of the motionEvents list, and when a scene is
   loaded unloads it, deletes it (flags 3; the pointer is re-read after the unload) and clears the
   pointer; then runs CoreTaskDestroy (flags 0) and frees the task when bit 0 of `flags` is set. */
void BtlDemoScenePlayerDtor(BtlDemoScenePlayer *player, u32 flags)
{
    if (player == NULL) {
        return;
    }
    player->base.vtable = g_btlDemoScenePlayerVtbl;
    CoreObjectListDeleteAll(&player->motionEvents);
    if (player->scene != NULL) {
        BtlDemoSceneUnload(player->scene);
        if (player->scene != NULL) {
            BtlDemoSceneDtor(player->scene, 3);
            player->scene = NULL;
        }
    }
    CoreTaskDestroy(&player->base, 0);
    if (flags & 1) {
        MemLock();
        MemFree(player, NULL, 0);
        MemUnlock();
    }
}
