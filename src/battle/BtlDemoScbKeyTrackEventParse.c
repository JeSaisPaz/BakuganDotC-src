// bdc 0x08909720 BtlDemoScbKeyTrackEventParse
#include "bdc.h"

/* Body parser (vtable `0x08af478c` slot `+0x14`) of `BtlDemoScbKeyTrackEvent`: `length` = body
   word 1; when `n` = body word 2 is non-zero, allocates n `BtlDemoScbKey`s from the low end of the
   heap (`CxxVecNew`, `keys` = NULL if the allocation fails), sets `keyCount` = n and fills key i
   with `{exponent[i], (int)(value[i] / 10^exponent[i])}` (raw `value[i]` when the exponent is 0;
   `powf`), where `value` is the n-word array at word 3 and `exponent` the n-word array after it.
   With n == 0, `keys` and `keyCount` are left untouched. Returns `n * 8 + 8`. */

int BtlDemoScbKeyTrackEventParse(BtlDemoScbKeyTrackEvent *ev, const s32 *body)
{
    const s32 *values = body + 3;
    const s32 *exponents = values + body[2];
    s32 count;

    ev->length = body[1];
    count = body[2];
    if (count != 0) {
        BtlDemoScbKey *keys = NULL;
        bool fromLow;
        void *block;
        u32 i;

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        block = MemAlloc(count * sizeof(BtlDemoScbKey) + 0x10 /* PSP: CxxVecNew array cookie */, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (block != NULL) {
            keys = CxxVecNew((u8 *)block + g_cxxVecCookieSize, count, sizeof(BtlDemoScbKey),
                             BtlDemoScbKeyCtor, 0);
        }
        ev->keys = keys;
        ev->keyCount = body[2];
        for (i = 0; i < (u32)body[2]; i++) {
            s32 exponent = exponents[i];

            ev->keys[i].exponent = exponent;
            if (exponent == 0) {
                ev->keys[i].value = values[i];
            } else {
                float scale = powf(10.0f, (float)exponents[i]);

                ev->keys[i].value = (s32)((float)values[i] / scale);
            }
        }
    }
    return body[2] * 8 + 8;
}
