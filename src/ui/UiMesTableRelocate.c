// bdc 0x088cc314 UiMesTableRelocate
#include "bdc.h"

/* Relocates a message table loaded from a `mes_*.bin` file in place: when the first word is an
   offset (smaller than the table address) adds the table base to each of the `first/4` entries
   and returns that count; when the first word is already an address (not smaller than the table)
   returns the distance `(first - table) / 4` without touching the table. */

u32 UiMesTableRelocate(u32 *table)
{
    uintptr_t base = (uintptr_t)table;
    u32 first = *table;
    u32 count;
    s32 i;

    if (first < base) {
        count = first >> 2;
        if (count != 0) {
            u32 *p = table;
            i = 0;
            do {
                i++;
                *p = *p + (u32)base;
                p++;
            } while (i < (s32)count);
        }
    } else {
        count = (first - (u32)base) >> 2;
    }
    return count;
}
