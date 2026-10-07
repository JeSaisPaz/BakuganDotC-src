// bdc 0x08870588 BtlBakuganTexLoaderTaskRequest
#include "bdc.h"

/* Starts a texture-load request on the Bakugan texture loader task: copies the four kind slots
   from `kinds`, sets the step to 1 and immediately runs the task's update virtual (vtable entry 2,
   `BtlBakuganTexLoaderTaskUpdate`). Called by `BtlMainPhaseLoad`. */
void BtlBakuganTexLoaderTaskRequest(BtlBakuganTexLoaderTask *task, s32 *kinds)
{
    const VtblEntry *update = &((const VtblEntry *)task->base.vtable)[2];
    int i;

    for (i = 0; i < 4; i++) {
        task->kinds[i] = kinds[i];
    }
    task->step = 1;
    ((void (*)(void *))update->fn)((u8 *)task + update->delta);
}
