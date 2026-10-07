// bdc 0x089e7ef8 GfxEffectChainInitLinkLengths
#include "bdc.h"

/* Allocates the per-link length array `+0x10` (`count` floats) and fills it with the chain's link
   length `+0x14`; sets the flag `+0x2c`. */

void GfxEffectChainInitLinkLengths(GfxEffectChain *chain)
{
    bool fromLow;
    float *lengths;
    int count = chain->count;
    int i;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    lengths = MemAlloc(count * sizeof(float), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    chain->linkLengths = lengths;
    for (i = 0; i < chain->count; i++) {
        chain->linkLengths[i] = chain->maxLinkLength;
    }
    chain->hasLinkLengths = 1;
}
