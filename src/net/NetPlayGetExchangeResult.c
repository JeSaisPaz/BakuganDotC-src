// bdc 0x0881b70c NetPlayGetExchangeResult
#include "bdc.h"

/* When the entry exchange has finished (state `+0xc0 == 6`), copies the 5-byte result `+0xd8` to
   `out` and returns 1; else returns 0. See `NetPlayStartExchange`. */

s32 NetPlayGetExchangeResult(NetPlay *self, u8 *out)
{
    int i;

    if (self->exchangeState == 6 && out != NULL) {
        for (i = 0; i < 5; i++) {
            out[i] = self->exchangeResult[i];
        }
        return 1;
    }
    return 0;
}
