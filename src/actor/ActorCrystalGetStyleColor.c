// bdc 0x08859868 ActorCrystalGetStyleColor
#include "bdc.h"

/* Writes the RGBA colour of crystal style `style` (0..8, table `g_actorCrystalStyleColors`,
   stride 0x30) into `out`, white for other values. */

void ActorCrystalGetStyleColor(float *out, int style)
{
    out[0] = 1.0f;
    out[1] = 1.0f;
    out[2] = 1.0f;
    out[3] = 1.0f;
    if (style >= 0 && style < 9) {
        const float *c = g_actorCrystalStyleColors[style];

        out[0] = c[0];
        out[1] = c[1];
        out[2] = c[2];
        out[3] = c[3];
    }
}
