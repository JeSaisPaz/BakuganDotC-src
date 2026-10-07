// bdc 0x088848d0 BtlInputGetCameraYawOffset
#include "bdc.h"

/* Returns `π/2 − yaw` of the active camera (`g_gfxActiveCamera`), wrapped once into (−π, π]
   (minus 2π when not ≤ π, plus 2π when ≤ −π): the offset that turns a screen-relative stick angle
   into a world heading. */
float BtlInputGetCameraYawOffset(void)
{
    float offset = 1.57079637f - g_gfxActiveCamera->yaw;

    if (!(offset <= 3.14159274f)) {
        return offset - 6.28318548f;
    }
    if (offset <= -3.14159274f) {
        offset = offset + 6.28318548f;
    }
    return offset;
}
