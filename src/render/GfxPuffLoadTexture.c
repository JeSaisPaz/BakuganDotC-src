// bdc 0x08828a90 GfxPuffLoadTexture
#include "bdc.h"

/* Caches the smoke texture `"kemuri1"` (`GfxFindTexture`) in `g_puffSmokeTexture` for the smoke-type
   sprite puffs (`GfxPuffCtor`). Called when a field/battle/demo loads its textures
   (`GameFieldPhaseLoad`, `BtlDemoStateLoad`, …). */

void GfxPuffLoadTexture(void)

{
  g_puffSmokeTexture = GfxFindTexture("kemuri1");
  return;
}

