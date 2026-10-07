// bdc 0x0890a578 UiLoadingInitShared
#include "bdc.h"

/* Lazily builds the shared objects of the now-loading screen (`UiLoadingCtor`) in
   `g_uiLoadingShared` (0x44 bytes, zeroed) when they do not exist yet; does nothing otherwise.
   All from the low end of the heap: a 0x80-byte sprite layer (`GfxSpriteLayerCtor`,
   `GfxSpriteLayerInitPool`, `sorted` set) holding the sprites of UI layout 0x2b
   (`UiLayoutGetCount`, `UiLayoutCreateSprites`; `sprites`, `spriteCount`), with the first 0x17
   sprites hidden (bit 0 of `flags` cleared) and the icon sprites 11..16 placed at the base
   positions of `g_loadingIconLayout` (their `+0x80` vector and scale/angle `+0x90..+0x9c`
   zeroed from the VFPU bank constant C720 = 0); the animation player and fab list cleared; the
   message box `box` (`UiTextBoxCtor`, packet depth 1610, `UiTextBoxCreatePrinter` with 512
   glyphs) and a 0x17000-byte buffer `tipBuffer` for the tip picture loaded by `UiLoadingCtor`. */

void UiLoadingInitShared(void)
{
  UiLoadingShared *shared;
  GfxSpriteLayer *layer;
  GfxSprite **sprites;
  GfxSprite *sprite;
  UiTextBox *box;
  void *buffer;
  bool low;
  s32 count;
  s32 i;

  if (g_uiLoadingShared != NULL) {
    return;
  }

  MemLock();
  low = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  shared = MemAlloc(sizeof(UiLoadingShared), NULL, 0);
  MemSetAllocFromLow(low);
  MemUnlock();
  g_uiLoadingShared = shared;
  memset(shared, 0, sizeof(UiLoadingShared));
  g_uiLoadingShared->spriteCount = UiLayoutGetCount(0x2b);

  MemLock();
  low = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  layer = MemAlloc(sizeof(GfxSpriteLayer), NULL, 0);
  MemSetAllocFromLow(low);
  MemUnlock();
  if (layer != NULL) {
    GfxSpriteLayerCtor(layer, 0);
  }
  g_uiLoadingShared->layer = layer;
  GfxSpriteLayerInitPool(layer, g_uiLoadingShared->spriteCount);
  g_uiLoadingShared->layer->sorted = 1;

  count = g_uiLoadingShared->spriteCount;
  MemLock();
  low = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprites = MemAlloc(count * sizeof(GfxSprite *), NULL, 0);
  MemSetAllocFromLow(low);
  MemUnlock();
  g_uiLoadingShared->sprites = sprites;
  UiLayoutCreateSprites(g_uiLoadingShared->layer, sprites, 0x2b);

  shared = g_uiLoadingShared;
  for (i = 0; i < 0x17; i++) {
    shared->sprites[i]->flags &= ~1u;
  }
  for (i = 11; i < 17; i++) {
    sprite = shared->sprites[i];
    sprite->posX = (float)g_loadingIconLayout[i - 11][0];
    sprite->posY = (float)g_loadingIconLayout[i - 11][1];
    sprite->maybe_billboardParams80[0] = 0.0f;
    sprite->maybe_billboardParams80[1] = 0.0f;
    sprite->maybe_billboardParams80[2] = 0.0f;
    sprite->maybe_billboardParams80[3] = 0.0f;
    sprite->scaleX = 0.0f;
    sprite->scaleY = 0.0f;
    sprite->scaleZ = 0.0f;
    sprite->angle = 0.0f;
  }
  shared->fab = NULL;
  shared->animAux18 = 0;
  shared->fabList = NULL;
  shared->animAux1c = 0;

  MemLock();
  low = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  box = MemAlloc(sizeof(UiTextBox), NULL, 0);
  MemSetAllocFromLow(low);
  MemUnlock();
  if (box != NULL) {
    UiTextBoxCtor(box);
  }
  shared = g_uiLoadingShared;
  shared->box = box;
  box->packetDepth = 1610.0f;
  UiTextBoxCreatePrinter(shared->box, 0x200);

  MemLock();
  low = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  buffer = MemAlloc(0x17000 /* PSP: tip picture TIM2 buffer, 92 KiB */, NULL, 0);
  MemSetAllocFromLow(low);
  MemUnlock();
  g_uiLoadingShared->tipBuffer = buffer;
}
