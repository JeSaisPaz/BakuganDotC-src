// bdc 0x0884b018 BtlInitAngle0758
#include "bdc.h"

/* Static initialiser (pointer at `0x08af5c30`): sets the global float `g_btlLockOnAngle` to
   0.1396 rad (8°), bits 0x3e0efa33. */

void BtlInitAngle0758(void)
{
    g_btlLockOnAngle = 0.139626309f; /* 0x3e0efa33 */
}
