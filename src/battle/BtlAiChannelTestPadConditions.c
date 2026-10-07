// bdc 0x0888f840 BtlAiChannelTestPadConditions
#include "bdc.h"

/* Calls each of the three `MemberFnPtr` predicates `padConds` of a command channel of
   `BtlAi` on the AI's virtual pad object (`pad`), skipping all-zero (empty) entries;
   every non-empty predicate is called even after one failed. Returns 1 unless some predicate
   returned 0, then 0. */
s32 BtlAiChannelTestPadConditions(BtlAi *self, BtlAiChannel *channel)
{
    s32 result = 1;
    int i;

    for (i = 0; i < 3; i++) {
        const MemberFnPtr *e = &channel->padConds[i];
        u8 *obj;
        void *fn;

        if (e->index == 0 && e->delta == 0 && e->pfn == NULL) {
            continue;
        }
        obj = (u8 *)&self->pad + e->delta;
        fn = e->pfn;
        /* GCC 2.x pointer-to-member: index != 0 is a virtual slot, pfn then holds the vptr offset */
        if (e->index != 0) {
            const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)e->pfn);
            const VtblEntry *entry = &vtbl[e->index];

            obj += entry->delta;
            fn = entry->fn;
        }
        if (((s32 (*)(void *))fn)(obj) == 0) {
            result = 0;
        }
    }
    return result;
}
