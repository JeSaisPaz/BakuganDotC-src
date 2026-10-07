// bdc 0x08a1d06c GmoLayerArrayCopy
#include "bdc.h"

/* Copies `count` 0x10-byte texture-layer records: when a copy is required (`flags & 0xf0401`, or
   flag 2 with a dynamic texture) carves a new array (`GmoPlanTakeRec10B`), copies each
   (`GmoLayerCopy`) and returns the new array; otherwise bumps each record's count (`+0`) and
   returns the original. Returns NULL when `arr` or `plan` is NULL. */

void *GmoLayerArrayCopy(void *arr, s32 count, u32 flags, void *plan)
{
    GmoLayer *src = arr;
    GmoLayer *out;
    s32 i;

    if (src == NULL || plan == NULL) {
        return NULL;
    }
    if ((flags & 0xf0401) == 0) {
        if (flags & 2) {
            for (i = 0; i < count; i++) {
                if (GmoTextureIsDynamic(src[i].texture) != 0) {
                    goto copy;
                }
            }
        }
        for (i = 0; i < count; i++) {
            src[i].f00++;
        }
        return src;
    }
copy:
    out = GmoPlanTakeRec10B(count, plan);
    for (i = 0; i < count; i++) {
        GmoLayerCopy(&out[i], &src[i], flags, plan);
    }
    return out;
}
