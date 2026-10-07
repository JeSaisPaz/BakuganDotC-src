// bdc 0x089be558 CoreLzssStreamInit
#include "bdc.h"

/* Initialises an incremental LZSS decoder state: zeroes the first 0xfee bytes of the 4 KiB ring
   window, sets the write position to 0xfee, resets the bit/phase words, and stores the source
   blob, the destination, the total output size from `CoreLzssGetSize` and the flag byte. */
void CoreLzssStreamInit(CoreLzssStream *self, const u8 *src, u8 *dst, u8 flag)
{
    memset(self->window, 0, 0xfee);
    self->ringPos = 0xfee;
    self->flagBits = 0;
    self->outCount = 0;
    self->outSize = CoreLzssGetSize(src);
    self->src = src;
    self->dst = dst;
    self->started = 0;
    self->phase = 0;
    self->flag = flag;
}
