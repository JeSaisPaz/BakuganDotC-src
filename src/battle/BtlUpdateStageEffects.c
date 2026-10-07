// bdc 0x0884c360 BtlUpdateStageEffects
#include "bdc.h"

/* Finds the battle main task (id 100) and runs `BtlMainUpdateStageEffects` on it; nothing when
   the task does not exist. */
void BtlUpdateStageEffects(void)
{
    BtlMain *self = CoreTaskFind(100);

    if (self != NULL) {
        BtlMainUpdateStageEffects(self);
    }
}
