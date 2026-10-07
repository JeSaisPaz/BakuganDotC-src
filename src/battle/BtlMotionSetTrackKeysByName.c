// bdc 0x08886480 BtlMotionSetTrackKeysByName
#include "bdc.h"

/* Looks up the motion data block `motion` in the GMO motion manager (`GmoMotionMgrGet`,
   `GmoMotionGetDataByName`, which loads `<motion>.bin` on a miss) and, when found, overwrites
   every key of its `'H'` tracks bound to `target` with the 3-float vector `value`
   (`BtlMotionDataSetTrackKeys`, half-float tracks re-packed). Returns 1 on success, 0 when the
   motion is missing. Used by `BtlBakuganInitMotions`. */
s32 BtlMotionSetTrackKeysByName(const char *motion, u16 target, float *value)
{
    GmoMotionInfo *data;

    data = GmoMotionGetDataByName(GmoMotionMgrGet(), motion);
    if (data != NULL) {
        BtlMotionDataSetTrackKeys(data, target, value);
        return 1;
    }
    return 0;
}
