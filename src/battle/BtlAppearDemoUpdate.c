// bdc 0x08900104 BtlAppearDemoUpdate
#include "bdc.h"

/* Update of the appear demo task (derived from `BtlDemo`): calls the `MemberFnPtr`
   `g_btlAppearDemoUpdateTable[state]` on the task (0 = `BtlAppearDemoStateLoad`, 1 =
   `BtlAppearDemoStatePlay`), going through the object's vtable when the entry's `index` is
   non-zero (GCC 2.x pointer-to-member call). */
void BtlAppearDemoUpdate(BtlDemo *task)
{
    const MemberFnPtr *entry = &g_btlAppearDemoUpdateTable[task->state];
    u8 *obj = (u8 *)task + entry->delta;
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
