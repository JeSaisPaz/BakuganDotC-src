// bdc 0x088d442c GameStageGetSunPosition
#include "bdc.h"

/* Unless `GameStageIs0Or13`, writes the current stage's sun position (table `g_gameStageSunTable`, x0.7,
   height x0.6) into `out` (16-byte aligned, 4 floats) and returns 1; otherwise 0. */

s32 GameStageGetSunPosition(float *out)
{
    s32 r;
    const float *e;

    r = GameStageIs0Or13();
    if (r == 0) {
        e = g_gameStageSunTable[g_gameStageIndex];
        out[0] = e[0];
        out[1] = e[1];
        out[2] = e[2];
        /* vscl.t + sv.q: lane 3 (out[3]) gets a stale VFPU value, left out */
        out[0] = out[0] * 0.7f;
        out[1] = out[1] * 0.7f;
        out[2] = out[2] * 0.7f;
        out[1] = out[1] * 0.6f;
        return 1;
    }
    return 0;
}
