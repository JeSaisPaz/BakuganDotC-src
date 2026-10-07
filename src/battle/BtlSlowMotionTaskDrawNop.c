// bdc 0x08849420 BtlSlowMotionTaskDrawNop
#include "bdc.h"

/* Empty draw method (vtable `0x08af18bc` slot 4) of the slow-motion task
   (`BtlSlowMotionTaskCtor`): just `jr ra`. */

void BtlSlowMotionTaskDrawNop(CoreTask *task)
{
    (void)task;
}
