// bdc 0x088eb170 GameEventSinFixed
#include "bdc.h"

/* Fixed-point sine for the event code: converts the 16-bit angle `angle` (1/65535 turns) to radians
   as `pi/2 - a` (wrapped into (-pi, pi]), and returns `sin` of it x 4096, rounded away from zero
   (+0.5 when the sine is > 0, -0.5 otherwise, then truncated), as an s16 (i.e. the cosine of the
   original angle in the event convention). The binary evaluates the (identical) sine again in
   each branch; it is computed once here. */

#define EVT_TWO_PI 0x1.921fb6p+2f  /* 6.28318548f */
#define EVT_PI 0x1.921fb6p+1f      /* 3.14159274f */
#define EVT_HALF_PI 0x1.921fb6p+0f /* 1.57079637f */
#define EVT_INV_65535 0x1.0001p-16f /* 1.52590219e-05f */

/* `pi/2 - a`, wrapped by one turn into (-pi, pi]. */
static inline float EvtSinWrap(float a)
{
    float r = -(a - EVT_HALF_PI);

    if (!(r <= EVT_PI)) {
        r = r - EVT_TWO_PI;
    } else if (r <= -EVT_PI) {
        r = r + EVT_TWO_PI;
    }
    return r;
}

s32 GameEventSinFixed(s32 angle)
{
    float a = (float)angle * EVT_TWO_PI * EVT_INV_65535;
    float s = __builtin_sinf(EvtSinWrap(a));

    if (!(s <= 0.0f)) {
        return (s32)(s16)(s32)(s * 4096.0f + 0.5f);
    }
    return (s32)(s16)(s32)(s * 4096.0f - 0.5f);
}
