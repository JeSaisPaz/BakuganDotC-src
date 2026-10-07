// bdc 0x08a25d48 GmoTextureMakeWritable
#include "bdc.h"

/* Copy-on-write for a texture record: gives it private copies of its shared pixel image, palette
   and GE command list so the instance can change them without touching the shared model data. Runs
   the two-pass image plan on a 0x70-byte stack object: `GmoImagePlanInit`, measure
   (`GmoTextureMeasureWritable`), `GmoImagePlanCommit`, build (`GmoTextureBuildWritable`),
   `GmoImagePlanFree`. Returns 1 on success, 0 on failure. */

s32 GmoTextureMakeWritable(void *tex, u32 flags) {
    u8 plan[0x70];
    s32 ok;

    GmoImagePlanInit(plan);
    ok = GmoTextureMeasureWritable(tex, flags, plan);
    if (ok != 0 && GmoImagePlanCommit(plan) != 0) {
        ok = GmoTextureBuildWritable(tex, flags, plan);
        GmoImagePlanFree(plan);
        return ok;
    }
    return 0;
}
