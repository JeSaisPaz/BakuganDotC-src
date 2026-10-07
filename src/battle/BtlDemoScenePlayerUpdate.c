// bdc 0x08905868 BtlDemoScenePlayerUpdate
#include "bdc.h"

/* Update (vtable slot `+0x14`) of the battle demo scene player task (task id 0x66, 0x70 bytes,
   vtable `0x08af46a4`, `BtlDemoScenePlayerCtor`): calls the handler for `state` through the
   `MemberFnPtr` table `g_btlDemoScenePlayerStateTable` (0 `BtlDemoScenePlayerStateStart`,
   1 `BtlDemoScenePlayerStateLoad`, 2 `BtlDemoScenePlayerStatePlay`, 3
   `BtlDemoScenePlayerStateDone`), adjusting `this` by the entry's `delta` and, for a virtual
   entry (`index != 0`), going through the vtable slot `index`. */
void BtlDemoScenePlayerUpdate(BtlDemoScenePlayer *player)
{
    const MemberFnPtr *e = &g_btlDemoScenePlayerStateTable[player->state];
    u8 *obj = (u8 *)player + e->delta;
    void *fn = e->pfn;

    if (e->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)e->pfn);
        const VtblEntry *entry = &vtbl[e->index];

        fn = entry->fn;
        obj += entry->delta;
    }
    ((void (*)(void *))fn)(obj);
}
