// bdc 0x08906b68 BtlDemoSceneEventExpireB
#include "bdc.h"

/* Event handler of the battle demo scene player task (`BtlDemoScenePlayer`) for another event kind; same body as `BtlDemoSceneEventExpire`: when the
   event's frame equals the player's current frame, deletes the event through its deleting
   destructor (vtable slot 1, flag 3). */
void BtlDemoSceneEventExpireB(BtlDemoScenePlayer *task, BtlDemoScbEvent *ev)
{
    if (ev->frame == task->frame && ev != NULL) {
        const VtblEntry *dtor = &((const VtblEntry *)ev->base.vtable)[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)ev + dtor->delta, 3);
    }
}
