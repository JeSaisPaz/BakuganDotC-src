// bdc 0x088ff6d4 BtlDemoDraw
#include "bdc.h"

/* Draw (vtable slot `+0x24`) of the battle intro demo task (task id 0x65, 0x790 bytes, vtable
   `0x08af45fc`, `BtlDemoCtor`): calls the `MemberFnPtr` `g_btlDemoDrawTable[state]` on the demo
   (0 = `BtlDemoDrawFade`, 1 = `BtlDemoDrawPlay`), going through the object's vtable when the
   entry's `index` is non-zero (GCC 2.x pointer-to-member call). */
void BtlDemoDraw(BtlDemo *demo)
{
    const MemberFnPtr *entry = &g_btlDemoDrawTable[demo->state];
    u8 *obj = (u8 *)demo + entry->delta;
    void *fn = entry->pfn;

    if (entry->index != 0) {
        /* pfn holds delta2, the offset of the vtable pointer inside the adjusted object. */
        const MemberFnPtr *slot =
            &(*(const MemberFnPtr **)(obj + (intptr_t)entry->pfn))[entry->index];
        fn = slot->pfn;
        obj += slot->delta;
    }
    ((void (*)(void *))fn)(obj);
}
