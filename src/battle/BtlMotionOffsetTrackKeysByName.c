// bdc 0x08886404 BtlMotionOffsetTrackKeysByName
#include "bdc.h"

/* Looks up the motion data block `motion` in the GMO motion manager (`GmoMotionMgrGet`,
   `GmoMotionGetDataByName`, which loads `<motion>.bin` on a miss) and, when found, adds `offset`
   with graded weights to the keys of its `'H'` tracks bound to `target`
   (`BtlMotionDataScaleTrackKeysGraded`: the k-th of n matching tracks gets (n − k)/n of it).
   Returns 1 on success, 0 when the motion is missing. Used by `BtlBakuganInitMotions` to patch
   the per-kind animations. */
s32 BtlMotionOffsetTrackKeysByName(const char *motion, u16 target, float *offset)
{
    GmoMotionInfo *data;

    data = GmoMotionGetDataByName(GmoMotionMgrGet(), motion);
    if (data != NULL) {
        BtlMotionDataScaleTrackKeysGraded(data, target, offset);
        return 1;
    }
    return 0;
}
