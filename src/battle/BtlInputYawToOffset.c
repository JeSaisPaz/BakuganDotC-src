// bdc 0x08884ab0 BtlInputYawToOffset
#include "bdc.h"

/* Returns `π/2 − yaw` wrapped to (−π, π]; the remote-player counterpart of
   `BtlInputGetCameraYawOffset` (the yaw comes with the network input).
   The first test is `!(offset <= π)`, so a NaN offset takes the −2π branch. */

float BtlInputYawToOffset(float yaw)
{
    float offset = 1.57079637f - yaw;        /* 0x3fc90fdb */

    if (!(offset <= 3.14159274f)) {          /* 0x40490fdb */
        return offset - 6.28318548f;         /* 0x40c90fdb */
    }
    if (offset <= -3.14159274f) {            /* 0xc0490fdb */
        offset = offset + 6.28318548f;
    }
    return offset;
}
