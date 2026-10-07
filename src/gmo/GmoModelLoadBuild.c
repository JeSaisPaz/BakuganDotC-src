// bdc 0x08a1c718 GmoModelLoadBuild
#include "bdc.h"

/* Build step of `GmoModelLoad`: returns 0 for a NULL model, plan or file, for `size` 1..0x1f
   (0 passes through the unsigned wrap), or when the `GmoFileHeader` signature is not
   `"OMG.00.1PSP"`. Otherwise walks the root chunk's children (none for a short-form root) for
   the type-3 model chunk number `index` (0-based); when found it resets the model
   (`GmoModelReset`), builds it from that chunk with a 0x50-byte build context whose first word
   is `plan` (`GmoModelBuild`), writes back the data cache and returns 1. Returns 0 when there
   is no such chunk. */
s32 GmoModelLoadBuild(void *model, const GmoFileHeader *gmo, u32 size, s32 index, void *plan)
{
    const GmoChunk *root;
    const u8 *end;
    const u8 *cur;
    void *planCtx[20]; /* 0x50-byte build context; only the first word (the plan) is set here */

    if (model == NULL || plan == NULL || gmo == NULL) {
        return 0;
    }
    /* size 1..0x1f is rejected; size 0 wraps and passes, as in the original. */
    if (size - 1 < 0x1f) {
        return 0;
    }
    if (gmo->magic != 0x2e474d4f) { /* "OMG." */
        return 0;
    }
    if (gmo->version != 0x312e3030) { /* "00.1" */
        return 0;
    }
    if (gmo->platform != 0x505350) { /* "PSP\0" */
        return 0;
    }
    root = &gmo->root;
    if (root == NULL) {
        return 0;
    }
    end = (const u8 *)root + root->size;
    if ((s16)root->type < 0) {
        cur = end; /* short form: no children */
    } else {
        cur = (const u8 *)root + root->childOffset;
    }
    for (; cur < end; cur += ((const GmoChunk *)cur)->size) {
        if ((((const GmoChunk *)cur)->type & 0x7fff) != 3) {
            continue;
        }
        index--;
        if (index == -1) {
            GmoModelReset(model);
            planCtx[0] = plan;
            GmoModelBuild(planCtx, cur, model);
            sceKernelDcacheWritebackAll();
            return 1;
        }
    }
    return 0;
}
