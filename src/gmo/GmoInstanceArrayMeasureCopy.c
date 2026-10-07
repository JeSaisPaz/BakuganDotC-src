// bdc 0x08a1d1f4 GmoInstanceArrayMeasureCopy
#include "bdc.h"

/* Measure pass of `GmoInstanceArrayCopy` over `count` `GmoInstance` records: returns 0 for a
   NULL array or plan and 1 without reserving anything unless `flags & 0x81`. Otherwise reserves
   the `count` 0x20-byte records (`GmoPlanReserveRec20`) and, per instance, what
   `GmoInstanceCopy` carves, each 4-aligned in pool 1 (pool 2 when `flags` bit 31 is set): the
   0x60-byte state block (0 bytes when `state` is NULL), the display list (`displayListWords * 4`
   bytes) and the vertex data (`vertexSize * vertexCount` bytes). Returns 1. */
s32 GmoInstanceArrayMeasureCopy(const GmoInstance *arr, s32 count, u32 flags, void *plan)
{
    const GmoInstance *inst = arr;
    s32 i;

    if (arr == NULL || plan == NULL) {
        return 0;
    }
    if ((flags & 0x81) == 0) {
        return 1;
    }
    GmoPlanReserveRec20(count, plan);
    for (i = 0; i < count; i++, inst++) {
        int pool = ((s32)flags < 0) ? 2 : 1;

        if (inst != NULL) {
            u32 vertexBytes = (u32)inst->vertexSize * inst->vertexCount;
            u32 dlBytes = (u32)inst->displayListWords << 2;
            int stateBytes = (inst->state != NULL) ? 0x60 : 0;

            GmoPlanReserve(plan, pool, 4, stateBytes);
            GmoPlanReserve(plan, pool, 4, dlBytes);
            GmoPlanReserve(plan, pool, 4, vertexBytes);
        }
    }
    return 1;
}
