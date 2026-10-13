// bdc 0x08a2c4a8 ActorPlayerRunState
#include "bdc.h"

/* Runs the player actor's current state (`ActorPlayerCtor`, vtable `0x08af38e4` entry 30, offset
   `+0xf4`): calls the `MemberFnPtr` `0x08a98ae0[actor+0x140]` on `actor + delta` (virtual when
   `vindex != 0`). */

void ActorPlayerRunState(ActorPlayer *self)
{
    const MemberFnPtr *member = &g_playerStateFns[self->base.state];
    u8 *obj = (u8 *)self + member->delta;
    void *fn = member->pfn;

    if (member->index != 0) {
        /* pfn holds delta2, the vptr offset in the object: 0x14, the CoreObject vtable, in every entry */
        const VtblEntry *vtbl = (const VtblEntry *)((CoreObject *)obj)->vtable;
        const VtblEntry *entry = &vtbl[member->index];

        fn = entry->fn;
        obj += entry->delta;
    }
    ((void (*)(void *))fn)(obj);
}
