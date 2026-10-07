// bdc 0x08a246ac SndSsVagDecodeBlock
#include "bdc.h"

/* Decodes one 16-byte VAG (PSX/PSP ADPCM) block into 28 16-bit samples at `out`: the header byte
   selects the predictor (filter coefficients from `g_sndSsVagCoefs`) and shift,
   `history[0..1]` carries the last two samples across blocks. Returns the block's flag byte (loop
   start/end markers; 7 = end, nothing decoded). */

u8 SndSsVagDecodeBlock(s16 *out, const u8 *block, s16 *history)
{
    u8 flag = block[1];
    int pred = block[0] >> 4;
    u32 shift = block[0] & 0xf;
    s8 c1 = g_sndSsVagCoefs[pred + 5];
    s8 c0 = g_sndSsVagCoefs[pred];
    int s0 = history[0];
    int s1 = history[1];
    int pos = 2;
    int n = 13;
    u8 ret = 7;

    if (flag != 7) {
        do {
            s8 b = (s8)block[pos];
            n--;
            pos++;
            s1 = ((((int)b << 0x1c) >> (shift + 10)) + s1 * c1 + s0 * c0) >> 6;
            if (s1 < -0x8000) s1 = -0x8000;
            if (s1 > 0x7fff) s1 = 0x7fff;
            *out = (s16)s1;
            s0 = (int)((((u32)(int)b & 0xfffffff0U) << (0xe - shift & 0x1f)) + s0 * c1 + s1 * c0) >> 6;
            if (s0 < -0x8000) s0 = -0x8000;
            if (s0 > 0x7fff) s0 = 0x7fff;
            out[1] = (s16)s0;
            out += 2;
        } while (n >= 0);
        history[1] = (s16)s1;
        history[0] = (s16)s0;
        ret = flag;
    }
    return ret;
}
