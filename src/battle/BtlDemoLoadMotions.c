// bdc 0x088ff534 BtlDemoLoadMotions
#include "bdc.h"

/* Loads the demo motions for the demo's `demoId` (`+0x654`) via `BtlDemoLoadMotion`: a variant 1 id
   (`BtlDemoIdIsVariant1`) loads motion 0x127, a variant 3 id (`BtlDemoIdIsVariant3`) motions 0x125
   and 0x126, any other id motion 0x124. Counterpart of `BtlDemoFreeMotions`. */

void BtlDemoLoadMotions(BtlDemo *demo)
{
    if (BtlDemoIdIsVariant1(demo->demoId)) {
        BtlDemoLoadMotion(demo, 0x127);
    } else if (BtlDemoIdIsVariant3(demo->demoId)) {
        BtlDemoLoadMotion(demo, 0x125);
        BtlDemoLoadMotion(demo, 0x126);
    } else {
        BtlDemoLoadMotion(demo, 0x124);
    }
}
