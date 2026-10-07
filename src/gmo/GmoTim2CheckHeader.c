// bdc 0x08a26ce4 GmoTim2CheckHeader
#include "bdc.h"

/* Returns true when `data` starts with the `TIM2` signature and `size` ≥ 16. */

s32 GmoTim2CheckHeader(const u32 *data, u32 size)
{
    s32 result;

    result = 0;
    if ((data != (u32 *)0x0) && (size >= 0x10)) {
        result = (*data == 0x324d4954);
    }
    return result;
}
