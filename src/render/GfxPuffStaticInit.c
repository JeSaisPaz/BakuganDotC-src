// bdc 0x0882a200 GfxPuffStaticInit
#include "bdc.h"

/* Static constructor of the sprite-puff translation unit (`GfxPuffCtor`; `GfxPuffKindShockwave`
   ends right before it): sets the vec4 `g_puffStaticVec` to `{0.05, 0.05, 0, 0}`, read by
   `GfxPuffKindDust`, `GfxPuffKindDarkSmoke` and `GfxPuffKindRisingSmoke`. */

void GfxPuffStaticInit(void)

{
  g_puffStaticVec[0] = 0.05f;
  g_puffStaticVec[1] = 0.05f;
  g_puffStaticVec[2] = 0.0f;
  g_puffStaticVec[3] = 0.0f;
  return;
}

