// bdc 0x08a297fc GfxEffectChainIsReady
#include "bdc.h"

/* Returns the ready byte `+0x34` of an effect chain (GfxEffectChainCtor clears it);
   GfxEffectRunCommands only steps the chain (GfxEffectChainUpdate) when it is set. */
u8 GfxEffectChainIsReady(GfxEffectChain *chain)
{
    return chain->ready;
}
