// bdc 0x089061c8 BtlDemoSceneEventExpire
#include "bdc.h"

/* Event handler of the battle demo scene player task (`BtlDemoScenePlayer`): when the
   event's frame equals the player's current frame, deletes the event through its deleting
   destructor (vtable slot 1, flag 3). */
void BtlDemoSceneEventExpire(BtlDemoScenePlayer *task, BtlDemoScbEvent *ev)
{
    if (ev->frame == task->frame && ev != NULL) {
        const VtblEntry *dtor = &((const VtblEntry *)ev->base.vtable)[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)ev + dtor->delta, 3);
    }
}
