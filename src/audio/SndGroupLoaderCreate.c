// bdc 0x089c1124 SndGroupLoaderCreate
#include "bdc.h"

/* Lazily creates the sound-effect group loader. When `g_soundGroupLoader` is NULL it allocates
   the 4-byte holder from the low heap (and zeroes it), then a 0x1c-byte `SndGroupLoader` built by
   `SndGroupLoaderInit` and stores the pointer in `*g_soundGroupLoader` (it stays NULL if the
   allocation fails). Does nothing when the holder already exists. */

void SndGroupLoaderCreate(void)
{
    bool wasLow;
    SndGroupLoader **holder;
    SndGroupLoader *self;
    SndGroupLoader *result;

    if (g_soundGroupLoader == NULL) {
        MemLock();
        wasLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        holder = MemAlloc(sizeof(SndGroupLoader *), NULL, 0);
        MemSetAllocFromLow(wasLow);
        MemUnlock();
        g_soundGroupLoader = holder;
        memset(holder, 0, sizeof(SndGroupLoader *));
        MemLock();
        wasLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        self = MemAlloc(sizeof(SndGroupLoader), NULL, 0);
        MemSetAllocFromLow(wasLow);
        MemUnlock();
        result = NULL;
        if (self != NULL) {
            SndGroupLoaderInit(self);
            result = self;
        }
        *g_soundGroupLoader = result;
    }
}
