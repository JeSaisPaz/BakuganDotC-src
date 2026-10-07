// bdc 0x089c9b08 CoreBitsetTest
#include "bdc.h"

/* Tests bit `bit` of the packed 32-bit-word bitset `bits`: true when
   `bits[bit >> 5] & (1 << (bit & 31))` is set. */
bool CoreBitsetTest(u32 bit, const u32 *bits)
{
    u32 mask = 1u << (bit & 31);

    return (bits[bit >> 5] & mask) == mask;
}
