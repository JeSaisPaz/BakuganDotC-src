// bdc 0x088b9e6c GameFieldCameraSetMode
#include "bdc.h"

/* Switches the mode of the field camera (`GameFieldCameraCtor`, embedded at `+0x20` of the field
   task; target `+0x2a0`, eye `+0x50`, look-at `+0x60`, mode `+0x2ac`): runs the current mode's exit
   handler from the `MemberFnPtr` table `g_gameFieldCameraExitFns` (only modes 4, 5, 7, 8, 9 have
   one), remembers the old mode in `+0x2a8` and stores `mode` in `+0x2ac`. */

void GameFieldCameraSetMode(GameFieldCamera *cam, s32 mode)
{
    const MemberFnPtr *member = &g_gameFieldCameraExitFns[cam->mode];
    s16 delta = member->delta;
    s16 index = member->index;
    void *fn = member->pfn;
    u8 *self;

    if (index == 0 && delta == 0 && fn == NULL) {
        if (cam->mode != cam->prevMode) {
            cam->prevMode = cam->mode;
        }
    } else {
        self = (u8 *)cam + delta;
        if (index != 0) {
            const VtblEntry *vtbl = *(const VtblEntry **)(self + (intptr_t)fn);
            const VtblEntry *entry = &vtbl[index];

            fn = entry->fn;
            self += entry->delta;
        }
        ((void (*)(void *))fn)(self);
        if (cam->mode != cam->prevMode) {
            cam->prevMode = cam->mode;
        }
    }
    cam->mode = mode;
}
