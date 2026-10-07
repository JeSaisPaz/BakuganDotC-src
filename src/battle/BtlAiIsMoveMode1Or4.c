// bdc 0x088931e0 BtlAiIsMoveMode1Or4
#include "bdc.h"

/* Returns 1 when movement mode `mode` is 1 or 4, else 0. Used by `BtlAiRunMoveRules` and
   `BtlAiMoveToGoalPoint`. */
s32 BtlAiIsMoveMode1Or4(BtlAi *self, s32 mode)
{
    (void)self;
    if (mode == 1 || mode == 4) {
        return 1;
    }
    return 0;
}
