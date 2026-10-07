// bdc 0x088a38fc ActorCrystalStandUpdate
#include "bdc.h"

/* Update method of the crystal stand (vtable `0x08af24d4` slot 7). While the battle camera task
   exists it sets `visible`: shown when task 0x14a exists; otherwise, when talk window 7 is active
   and window 8 is not, shown only if the camera task 100 exists without task flag 1; in every other
   case shown unless a battle demo runs (`BtlIsDemoRunning`). Without the camera task `visible` is
   left as is. Then it makes the collider non-solid (flags |= 6) while hidden or almost transparent
   (`ambient[3]` <= 0.3), solid (flags &= ~6) otherwise, checks the fixed stand point on arenas >= 0x24
   (`ActorCrystalStandCheckOrigin`) and steps the model animation
   (`GfxModelUpdateAndApplyMotion`). */

void ActorCrystalStandUpdate(ActorCrystalStand *stand)
{
    CollisionCollider *collider;
    void *task;
    bool show;

    if (BtlCameraTaskExists() != 0) {
        if (CoreTaskExists(0x14a) != 0) {
            stand->base.visible = 1;
            goto apply;
        }
        UiGetTalkTask();
        if (UiGetWindowActive(7) != 0) {
            UiGetTalkTask();
            if (UiGetWindowActive(8) == 0) {
                show = false;
                task = CoreTaskFind(100);
                if (task != NULL && !CoreTaskHasFlags(task, 1)) {
                    show = true;
                }
                if (show) {
                    stand->base.visible = 1;
                } else {
                    stand->base.visible = 0;
                }
                goto apply;
            }
        }
        if (!BtlIsDemoRunning()) {
            stand->base.visible = 1;
        } else {
            stand->base.visible = 0;
        }
    }
apply:
    collider = stand->collider;
    if (stand->base.visible == 0 || stand->base.ambient[3] <= 0.300000012f) {
        if (collider != NULL) {
            collider->flags = collider->flags | 6;
        }
    } else if (collider != NULL) {
        collider->flags = collider->flags & ~6u;
    }
    if (g_btlArenaIndex >= 0x24) {
        ActorCrystalStandCheckOrigin(stand);
    }
    GfxModelUpdateAndApplyMotion(&stand->base);
}
