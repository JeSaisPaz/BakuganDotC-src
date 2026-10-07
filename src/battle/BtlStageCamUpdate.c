// bdc 0x08903984 BtlStageCamUpdate
#include "bdc.h"

/* Update (vtable slot `+0x14`) of the stage camera demo task (`BtlStageCam`, task id 0x6b,
   `BtlStageCamCtor`): calls the `MemberFnPtr` `g_btlStageCamStateFns[task->state]`
   (`g_btlStageCamStateFns`: 0 = `BtlStageCamStateStart`, 1 = `BtlStageCamStatePlay`,
   2 = `BtlStageCamStateEnd`) on the task. */
void BtlStageCamUpdate(BtlStageCam *task)
{
    const MemberFnPtr *member = &g_btlStageCamStateFns[task->state];
    u8 *self = (u8 *)task + member->delta;
    void *fn = member->pfn;

    /* GCC 2.x pointer-to-member: index != 0 is a virtual slot, pfn then holds the vptr offset */
    if (member->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(self + (intptr_t)member->pfn);
        const VtblEntry *entry = &vtbl[member->index];

        fn = entry->fn;
        self += entry->delta;
    }
    ((void (*)(void *))fn)(self);
}
