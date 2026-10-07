// bdc 0x08886388 BtlMotionAddTrackOffsetByName
#include "bdc.h"

/* Looks up motion data `name` through the motion manager and, when found, adds `offset` to its
   'H' tracks bound to `target` (BtlMotionDataOffsetTrackKeys); returns 1, or 0 when the motion is
   missing. */

int BtlMotionAddTrackOffsetByName(char *name, u32 target, float *offset)
{
    void *motion = GmoMotionGetDataByName(GmoMotionMgrGet(), name);

    if (motion != NULL) {
        BtlMotionDataOffsetTrackKeys(motion, target, offset);
        return 1;
    }
    return 0;
}
