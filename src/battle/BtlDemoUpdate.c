// bdc 0x088ff654 BtlDemoUpdate
#include "bdc.h"

/* Update (vtable slot `+0x14`) of the battle intro demo task (`BtlDemo`, task id 0x65,
   `BtlDemoCtor`): calls the `MemberFnPtr` `g_btlDemoStateFns[demo->state]`
   (`g_btlDemoStateFns`: 0 = `BtlDemoStateLoad`, 1 = `BtlDemoStatePlay`) on the task, then
   runs `BtlUpdateStageEffects` when the battle camera task exists (`BtlCameraTaskExists`). */
void BtlDemoUpdate(BtlDemo *demo)
{
    const MemberFnPtr *member = &g_btlDemoStateFns[demo->state];
    u8 *self = (u8 *)demo + member->delta;
    void *fn = member->pfn;

    /* GCC 2.x pointer-to-member: index != 0 is a virtual slot, pfn then holds the vptr offset */
    if (member->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(self + (intptr_t)member->pfn);
        const VtblEntry *entry = &vtbl[member->index];

        fn = entry->fn;
        self += entry->delta;
    }
    ((void (*)(void *))fn)(self);

    if (BtlCameraTaskExists()) {
        BtlUpdateStageEffects();
    }
}
