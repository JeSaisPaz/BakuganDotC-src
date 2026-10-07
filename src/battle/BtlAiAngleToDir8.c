// bdc 0x0888f6e8 BtlAiAngleToDir8
#include "bdc.h"

/* Quantises an angle (radians) to one of eight 16-bit directions `0x2000 * n`: the angle is
   doubled, converted to degrees and halved (negative values map to `(360 + d) * 0.5 + 180`,
   i.e. wrapped into 0..360), then the closed sectors [22.5 + 45 * (n - 1), 22.5 + 45 * n] for
   n = 1..7 are scanned in order; the first that contains it gives `0x2000 * n` (a shared bound
   such as 67.5 goes to the lower n; 22.5 itself gives 0x2000), anything else (the forward sector
   below 22.5 or above 337.5, NaN) gives 0. */
u16 BtlAiAngleToDir8(float angle)
{
    u16 dirs[8];
    float deg;
    float lo;
    float hi;
    u8 n;

    dirs[0] = 0;
    dirs[1] = 0x2000;
    dirs[2] = 0x4000;
    dirs[3] = 0x6000;
    dirs[4] = 0x8000;
    dirs[5] = 0xa000;
    dirs[6] = 0xc000;
    dirs[7] = 0xe000;
    deg = angle * 2.0f * 57.2957802f;
    if (deg < 0.0f) {
        deg = (360.0f - -deg) * 0.5f + 180.0f;
    } else {
        deg = deg * 0.5f;
    }
    n = 1;
    lo = 22.5f;
    for (;;) {
        hi = lo + 45.0f;
        if (!(deg < lo) && deg <= hi) {
            return dirs[n];
        }
        n++;
        if (n >= 8) {
            return 0;
        }
        lo = hi;
    }
}
