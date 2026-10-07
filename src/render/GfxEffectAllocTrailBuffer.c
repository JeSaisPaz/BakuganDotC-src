// bdc 0x0881e048 GfxEffectAllocTrailBuffer
#include "bdc.h"

/* Allocates (low heap) a `3 × count` float buffer for the mesh object (`GfxMeshObjCtor`) attached
   to the `effect` (`meshObj`), whose embedded rope/trail chain is `effectChain`
   (`GfxEffectChainCtor`), stores it in the mesh object's `vertexBuffer` and sets bit 0 of its
   `flags`. For effect kinds 0xb/0xc (`kind`) the second third is filled with 1.0 and the last
   third with 0.0 (the first third is left as allocated); for other kinds it first sets up the
   chain's per-link lengths (`GfxEffectChainInitLinkLengths`) and fills the first two thirds
   with 1.0 and the last third with 0.0. The allocation result is not checked for NULL. Called by
   `GfxEffectRunCommands`. */

void GfxEffectAllocTrailBuffer(GfxEffect *effect, s32 count)
{
  s32 total = count * 3;
  s32 twoThirds = count * 2;
  bool wasLow;
  float *buf;
  s32 i;

  if (effect->kind == 0xb || effect->kind == 0xc) {
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    buf = MemAlloc(total * (s32)sizeof(float), NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    for (i = count; i < twoThirds; i++) {
      buf[i] = 1.0f;
    }
  } else {
    GfxEffectChainInitLinkLengths(&effect->meshObj->effectChain);
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    buf = MemAlloc(total * (s32)sizeof(float), NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    for (i = 0; i < twoThirds; i++) {
      buf[i] = 1.0f;
    }
  }
  for (i = twoThirds; i < total; i++) {
    buf[i] = 0.0f;
  }
  effect->meshObj->vertexBuffer = buf;
  effect->meshObj->flags |= 1;
}
