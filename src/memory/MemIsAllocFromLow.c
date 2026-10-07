// bdc 0x089d8174 MemIsAllocFromLow
#include "bdc.h"

/* Returns whether the game heap currently allocates from low addresses: true when `g_memMng`
   exists and bit0 of `MemMng``.flags` is set, false otherwise. See `MemSetAllocFromLow`. */
bool MemIsAllocFromLow(void)
{
    return g_memMng != NULL && (g_memMng->flags & 1) != 0;
}
