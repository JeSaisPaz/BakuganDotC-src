// bdc 0x08a1c9a8 GmoAttrMeasureCopy
#include "bdc.h"

/* Measure pass for copying a 0x40-byte material attribute record: when `src != dst` and `flags &
   0x201`, reserves its private 0x40-byte data block (when `+0xc` is set) and 0x10-byte block (when
   `+0x28` is set). Returns 1 (0 for NULL arguments). */

s32 GmoAttrMeasureCopy(void *dst, const void *src, u32 flags, void *plan)
{
    const GmoAttr *s = (const GmoAttr *)src;
    int size;
    int size2;

    if (s == NULL || plan == NULL) {
        return 0;
    }
    if (src == dst || (flags & 0x201) == 0) {
        return 1;
    }
    size2 = 0x10;
    size = 0x40;
    if (s->block == NULL) {
        size = 0;
    }
    if (s->data == NULL) {
        size2 = 0;
    }
    GmoPlanReserve(plan, 0, 0x40, size);
    GmoPlanReserve(plan, 0, 0x10, size2);
    return 1;
}
