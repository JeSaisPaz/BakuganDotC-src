// bdc 0x089408fc UiScreen390ReleaseEffect
#include "bdc.h"

/* Releases the effect `+0x8c` of `UiScreen390` from its layer
   (`UiSpriteLayerRelease` with its owner `+0x214`), if any. */

void UiScreen390ReleaseEffect(UiScreen *screen)

{
  GfxEffect *sprite;
  
  sprite = ((UiScreen390 *)screen)->effect;
  if (sprite != (GfxEffect *)0x0) {
    UiSpriteLayerRelease(sprite->mgr,sprite);
  }
  return;
}

