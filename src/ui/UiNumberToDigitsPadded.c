// bdc 0x089a4c34 UiNumberToDigitsPadded
#include "bdc.h"

/* Like `UiNumberToDigits` but keeps leading zeros: writes exactly `digits` decimal digits of
   `value` to `out` followed by `terminator`. Only caller `UiPasscodeInitAnswer`. */

void UiNumberToDigitsPadded(u8 *out, u32 value, u8 digits, u8 terminator)
{
    u8 buf[16];
    u8 *p = buf;
    u32 pow = 1;
    u32 i;
    u32 n = digits;
    u32 d;

    for (i = 1; i < n; i++) {
        pow = pow * 10;
    }
    d = 0;
    while (pow != 1) {
        *p = (u8)(value / pow);
        value = value - *p * pow;
        d++;
        p = buf + d;
        pow = pow / 10;
    }
    *p = (u8)value;

    i = 0;
    p = out;
    if (i < n) {
        do {
            *p = buf[i];
            i++;
            p = out + i;
        } while (i < n);
    }
    *p = terminator;
}
