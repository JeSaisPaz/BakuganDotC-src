// bdc 0x088ea8d4 Atan2Fixed16
#include "bdc.h"

/* Integer `atan2`: returns the angle of the vector (`x`, `y`) as an unsigned 16-bit binary angle
   (0x10000 = full turn, so 0x8000 = pi, 0x4000 = pi/2). Folds the vector into one octant by sign
   and magnitude comparisons (exact diagonals and axes return 0, 0x2000, 0x4000, 0x6000, 0x8000,
   0xa000, 0xc000 or 0xe000 directly), divides the smaller magnitude by the larger as a 64-bit
   20.12 fixed-point quotient (`__divdi3`) and adds or subtracts the `g_atanFixed16Table`
   entry for it to the octant's base angle. */
u16 Atan2Fixed16(s32 x, s32 y)
{
    s32 num;  /* smaller magnitude */
    s32 den;  /* larger magnitude */
    s32 base; /* octant base angle */
    bool add; /* add the table angle to base (else subtract) */
    s32 index;

    if (x > 0) {
        if (y > 0) {
            if (x < y) {
                num = x; den = y; base = 0; add = true;
            } else if (y < x) {
                num = y; den = x; base = 0x4000; add = false;
            } else {
                return 0x2000;
            }
        } else if (y == 0) {
            return 0x4000;
        } else {
            s32 ny = -y;
            if (ny < x) {
                num = ny; den = x; base = 0x4000; add = true;
            } else if (x < ny) {
                num = x; den = ny; base = 0x8000; add = false;
            } else {
                return 0x6000;
            }
        }
    } else if (x == 0) {
        return (u16)(y >= 0 ? 0 : 0x8000);
    } else {
        s32 nx = -x;
        if (y < 0) {
            s32 ny = -y;
            if (nx < ny) {
                num = nx; den = ny; base = 0x8000; add = true;
            } else if (ny < nx) {
                num = ny; den = nx; base = 0xc000; add = false;
            } else {
                return (u16)0xa000;
            }
        } else if (y > 0) {
            if (y < nx) {
                num = y; den = nx; base = 0xc000; add = true;
            } else if (nx < y) {
                num = nx; den = y; base = 0; add = false;
            } else {
                return (u16)0xe000;
            }
        } else {
            return (u16)0xc000;
        }
    }

    if (den == 0) {
        return 0;
    }
    index = (s32)(((s64)num << 12) / (s64)den) >> 5;
    if (add) {
        return (u16)(base + g_atanFixed16Table[index]);
    }
    return (u16)(base - g_atanFixed16Table[index]);
}
