// bdc 0x089e7cc0 GfxEffectChainCtor
#include "bdc.h"

/* Constructor of an effect rope/trail chain (`+0x0` positions, `+0x4` velocities, `+0x8` previous
   positions — vec4 arrays of `count` points; `+0xc` anchor position, `+0x10` optional per-link
   lengths, `+0x14` max link length, `+0x18..+0x20` damping/stiffness/gravity, `+0x24` count,
   `+0x28` physics on, `+0x2e` two-sided constraint pass): clears the point buffer, link-length
   array, extra buffer `+0x30` and the ready byte `+0x34`. */

void *GfxEffectChainCtor(GfxEffectChain *chain)

{
  chain->points = (ScePspFVector4 *)0x0;
  chain->linkLengths = (float *)0x0;
  chain->extraBuffer = (void *)0x0;
  chain->ready = '\0';
  return chain;
}

