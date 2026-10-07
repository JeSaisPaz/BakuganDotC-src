// bdc 0x089c9b88 CoreBitsetToggle
#include "bdc.h"

/* Toggles bit `bit` of the packed 32-bit-word bitset `bits`. */
void CoreBitsetToggle(u32 bit, u32 *bits)
{
    bits[bit >> 5] ^= 1u << (bit & 31);
}
