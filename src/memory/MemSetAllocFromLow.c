// bdc 0x089d813c MemSetAllocFromLow
#include "bdc.h"

/* Sets (`fromLow != 0`) or clears bit0 of `MemMng``.flags` of `g_memMng`, selecting the
   placement policy of `MemAlloc`: set = first fit from the bottom of the pool, clear =
   highest-address fit carved from the top. No-op without a heap. */
void MemSetAllocFromLow(bool fromLow)
{
    if (g_memMng == NULL) {
        return;
    }
    if (fromLow) {
        g_memMng->flags |= 1;
    } else {
        g_memMng->flags &= ~1u;
    }
}
