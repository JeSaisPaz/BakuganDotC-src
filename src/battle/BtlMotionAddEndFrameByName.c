// bdc 0x088864fc BtlMotionAddEndFrameByName
#include "bdc.h"

/* Looks up the motion data block `motion` (`GmoMotionGetDataByName`) and adds `delta` to its
   end frame (`GmoMotionInfo` `endFrame`). Returns 1 on success, 0 when the motion is missing.
   Used by `BtlBakuganInitMotions`. */

s32 BtlMotionAddEndFrameByName(float delta, const char *motion)
{
    GmoMotionInfo *info;

    info = (GmoMotionInfo *)GmoMotionGetDataByName(GmoMotionMgrGet(), motion);
    if (info != NULL) {
        info->endFrame = info->endFrame + delta;
        return 1;
    }
    return 0;
}
