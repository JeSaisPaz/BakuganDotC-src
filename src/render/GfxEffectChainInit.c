// bdc 0x089e7dd0 GfxEffectChainInit
#include "bdc.h"

/* Sets up the effect rope/trail chain `chain` with `count` points and maximum link length
   `segLen`, following `anchor`: stores count, max link length and anchor, allocates one low-heap
   block of `count * 0x30` bytes (`MemAlloc` under `MemLock`, placement forced to low and then
   restored) holding the `points`, `velocities` and `prevPoints` arrays, turns physics on with the
   default damping 0.1 / stiffness 0.73 / gravity 0.3, clears `hasLinkLengths`, `flag2d` and
   `twoSided`, places all points at `anchor` (or at `g_gfxVecZero` when NULL) with
   `GfxEffectChainReset` and marks the chain ready. The allocation result is not checked. */

void GfxEffectChainInit(float segLen, GfxEffectChain *chain, s32 count, const ScePspFVector4 *anchor)

{
  bool fromLow;
  ScePspFVector4 *buf;

  chain->count = count;
  chain->maxLinkLength = segLen;
  chain->anchor = anchor;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  buf = MemAlloc(count * 3 * sizeof(ScePspFVector4), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  chain->points = buf;
  chain->velocities = buf + count;
  chain->prevPoints = buf + count * 2;
  chain->physics = 1;
  chain->damping = 0.1f;
  chain->stiffness = 0.73f;
  chain->gravity = 0.3f;
  chain->hasLinkLengths = 0;
  chain->flag2d = 0;
  chain->twoSided = 0;
  if (anchor == NULL) {
    GfxEffectChainReset(chain, &g_gfxVecZero);
  }
  else {
    GfxEffectChainReset(chain, anchor);
  }
  chain->ready = 1;
}
