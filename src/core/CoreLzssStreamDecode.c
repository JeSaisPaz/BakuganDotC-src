// bdc 0x089be774 CoreLzssStreamDecode
#include "bdc.h"

/* Resumable LZSS decode step: consumes at most `inBytes` source bytes from a state set up by
   `CoreLzssStreamInit` (skipping the 4-byte size header on the first call), writing output and
   the ring window, and saves the decoder phase, flags, pointers and output count so the next call
   continues mid-token. Returns 1 while more output is expected, 0 when the full size (`outSize`)
   has been produced.

   Phases: 1 = flag bits loaded, 2 = literal written, 3 = first match byte read, 4 = match copied.
   A literal that reaches `outSize` is not written to the window; a match is copied in full
   (`matchLen + 1` bytes) before the size test. */
int CoreLzssStreamDecode(CoreLzssStream *self, u32 inBytes)
{
    s32 phase = self->phase;
    u32 consumed = 0;
    int more = 1;
    const u8 *src = self->src;
    u32 matchPos;
    s32 matchLen;
    u32 ringPos;
    u32 flagBits;
    u32 outCount;

    if (self->started == 0) {
        src += 4;
        self->started = 1;
        self->src = src;
        consumed = 4;
    }
    matchPos = self->matchPos;
    matchLen = self->matchLen;
    ringPos = self->ringPos;
    flagBits = self->flagBits;
    outCount = self->outCount;

    for (;;) {
        u8 b;
        s32 i;

        if (phase != 3) {
            if (phase != 1) {
                flagBits >>= 1;
                phase = 1;
                if ((flagBits & 0x100) == 0) {
                    /* flag byte exhausted: load the next one with the 0xff00 sentinel */
                    flagBits = *src++ | 0xff00;
                    consumed++;
                    if (consumed >= inBytes) {
                        break;
                    }
                }
            }
            if (flagBits & 1) {
                /* literal byte */
                b = *src++;
                consumed++;
                *self->dst++ = b;
                outCount++;
                phase = 2;
                if (outCount >= self->outSize) {
                    more = 0;
                    break;
                }
                self->window[ringPos] = b;
                ringPos = (ringPos + 1) & 0xfff;
                if (consumed >= inBytes) {
                    break;
                }
                continue;
            }
            /* match: low byte of the window position */
            matchPos = *src++;
            consumed++;
            phase = 3;
            if (consumed >= inBytes) {
                break;
            }
        }
        /* match second byte: high position nibble and length */
        b = *src++;
        consumed++;
        phase = 4;
        matchPos |= (u32)(b & 0xf0) << 4;
        matchLen = (b & 0xf) + 2;
        for (i = 0; i <= matchLen; i++) {
            u8 c = self->window[(matchPos + i) & 0xfff];

            *self->dst++ = c;
            self->window[ringPos] = c;
            ringPos = (ringPos + 1) & 0xfff;
        }
        outCount += matchLen + 1;
        if (outCount >= self->outSize) {
            more = 0;
            break;
        }
        if (consumed >= inBytes) {
            break;
        }
    }

    self->phase = phase;
    self->matchPos = matchPos;
    self->matchLen = matchLen;
    self->ringPos = ringPos;
    self->flagBits = flagBits;
    self->outCount = outCount;
    self->src = src;
    return more;
}
