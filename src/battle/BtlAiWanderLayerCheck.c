// bdc 0x088959c4 BtlAiWanderLayerCheck
#include "bdc.h"

/* Check method of behaviour layer 3 of `BtlAi` (wander, `MemberFnPtr` at
   `0x08a80310`): when the layer's virtual entry 2 (IsActive) returns 0, activates it (`active`) if
   there is no target, none of move flags 0x1000/0x2000/0x4000 is set and the seek-item layer's
   IsActive returns 0, and also once `idleTime` is not below 3 s (with or without a target). Returns
   the wander layer's IsActive. */
s32 BtlAiWanderLayerCheck(BtlAi *self)
{
    const VtblEntry *entry;

    entry = &self->wander.base.vtbl[2];
    if (((s32 (*)(void *))entry->fn)((u8 *)&self->wander + entry->delta) == 0) {
        if (self->target == NULL) {
            u32 moveFlags = self->moveFlags;
            u8 idle = 1;

            if ((moveFlags & 0x4000) != 0) {
                idle = 0;
            }
            if ((moveFlags & 0x2000) != 0) {
                idle = 0;
            }
            if ((moveFlags & 0x1000) != 0) {
                idle = 0;
            }
            entry = &self->seekItem.base.vtbl[2];
            if (((s32 (*)(void *))entry->fn)((u8 *)&self->seekItem + entry->delta) != 0) {
                idle = 0;
            }
            if (idle) {
                self->wander.base.active = 1;
            }
        }
        if (!(self->idleTime < 3.0f)) {
            self->wander.base.active = 1;
        }
    }
    entry = &self->wander.base.vtbl[2];
    return ((s32 (*)(void *))entry->fn)((u8 *)&self->wander + entry->delta);
}
