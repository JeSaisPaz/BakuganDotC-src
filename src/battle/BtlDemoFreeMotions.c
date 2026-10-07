// bdc 0x088fef0c BtlDemoFreeMotions
#include "bdc.h"

/* Frees the demo motions loaded by `BtlDemoLoadMotions` for the demo's `demoId` via
   `BtlDemoFreeMotion`: a variant 1 id (`BtlDemoIdIsVariant1`) frees motion 0x127, a variant 3
   id (`BtlDemoIdIsVariant3`) motions 0x125 and 0x126, any other id motion 0x124. */
void BtlDemoFreeMotions(BtlDemo *demo)
{
    if (BtlDemoIdIsVariant1(demo->demoId)) {
        BtlDemoFreeMotion(demo, 0x127);
    } else if (BtlDemoIdIsVariant3(demo->demoId)) {
        BtlDemoFreeMotion(demo, 0x125);
        BtlDemoFreeMotion(demo, 0x126);
    } else {
        BtlDemoFreeMotion(demo, 0x124);
    }
}
