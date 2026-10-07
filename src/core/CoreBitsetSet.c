// bdc 0x089c9b34 CoreBitsetSet
#include "bdc.h"

/* Sets bit `bit` of the packed 32-bit-word bitset `bits`. */
void CoreBitsetSet(u32 bit, u32 *bits)
{
    bits[bit >> 5] |= 1u << (bit & 31);
}
