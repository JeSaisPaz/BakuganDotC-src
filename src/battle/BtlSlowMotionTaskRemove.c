// bdc 0x088494e8 BtlSlowMotionTaskRemove
#include "bdc.h"

/* Removes the slow-motion task (id 0x14b, `CoreTaskRemoveById`). */
void BtlSlowMotionTaskRemove(void)
{
    CoreTaskRemoveById(0x14b);
}
