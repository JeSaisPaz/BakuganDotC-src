// bdc 0x089bfcdc SndApplyFalloffCurve
#include "bdc.h"

/* Maps a normalised distance `t` (0 = at the emitter, 1 = at the audible radius) through one of
   five falloff curves selected by `curveId`: `1` ease `cos(t*pi/2) - (1 - t) + t`, `-1` linear `1 -
   t`, `-2` `2(1 - t) - cos(t*pi/2)`, `-3` circular `1 - sqrt(1 - (t - 1)^2)`, anything else
   identity `t`. The cosine is `vcos.s` of `t*pi/2` scaled by the bank's 2/pi (S703), i.e.
   `cos(t*pi/2)` in radians. */

float SndApplyFalloffCurve(float t, s32 curveId)
{
    if (curveId == 1) {
        float c = __builtin_cosf(t * 1.5707964f);
        return (c - (1.0f - t)) + t;
    }
    if (curveId == -1) {
        return 1.0f - t;
    }
    if (curveId == -2) {
        float c = __builtin_cosf(t * 1.5707964f);
        return (1.0f - t) * 2.0f - c;
    }
    if (curveId == -3) {
        float d = t - 1.0f;
        t = 1.0f - __builtin_sqrtf(1.0f - d * d);
    }
    return t;
}
