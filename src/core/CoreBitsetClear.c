// bdc 0x089c9b5c CoreBitsetClear
#include "bdc.h"

/* Clears bit `bit` of the packed 32-bit-word bitset `bits` (`bits[bit >> 5] &= ~(1 << (bit &
   31))`). Called by `ScriptOpStartNextPlaythrough` among others. */
void CoreBitsetClear(u32 bit, u32 *bits)
{
    bits[bit >> 5] &= ~(1u << (bit & 31));
}
