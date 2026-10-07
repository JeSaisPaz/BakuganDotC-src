// bdc 0x089a5c14 UiMenuFlagsModify
#include "bdc.h"

/* Modifies bit `bit` of profile word 0x35 (a menu flag mask): `op` 0 clears it, 1 sets it, any other
   value clears the whole word. */

void UiMenuFlagsModify(u8 op, u32 bit)
{
    u32 mask = bit & 0xffff;
    u32 flags;

    if (op == 0) {
        flags = SaveProfileGetWord(SaveGetProfile(), 0x35);
        flags = (flags & 0xffff) & ~(1 << (mask & 0x1f));
        SaveProfileSetWord(SaveGetProfile(), 0x35, flags & 0xffff);
    } else if (op < 2) {
        flags = SaveProfileGetWord(SaveGetProfile(), 0x35);
        flags = (flags & 0xffff) | (1 << (mask & 0x1f));
        SaveProfileSetWord(SaveGetProfile(), 0x35, flags & 0xffff);
    } else {
        SaveProfileSetWord(SaveGetProfile(), 0x35, 0);
    }
}
