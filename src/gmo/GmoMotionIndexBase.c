// bdc 0x089d91d8 GmoMotionIndexBase
#include "bdc.h"

/* Returns `GmoMotionCount() + offset`, the registry index a newly added motion entry will get, or
   -1 when the motion manager does not exist (`GmoMotionMgrExists`). */
s32 GmoMotionIndexBase(s32 offset)
{
    if (!GmoMotionMgrExists()) {
        return -1;
    }
    /* The manager is passed in a0 to GmoMotionCount, whose signature takes no argument. */
    GmoMotionMgrGet();
    return GmoMotionCount() + offset;
}
