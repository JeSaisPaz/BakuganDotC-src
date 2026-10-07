// bdc 0x0880969c UiLanguageSelectCtor
#include "bdc.h"

/* Constructor of the first-boot language-selection screen `UiLanguageSelect` (task id 199 = 0xc7,
   0x60 bytes, vtable `g_uiLanguageSelectVtbl`): disables frame skip, enables stick→D-pad
   emulation on `g_padState`, creates a sprite layer (`layer`) and a 0x6c-byte sprite array
   (`sprites`), clears the state, cursor and textures, sets the 7 per-row scales to 1.0, clears the
   script result (`UiLanguageSelectSetResult` 0) and, when a profile exists, preselects the row of
   the current language: it reads `PadGetLanguage`, keeps it if supported (1-6, 12; otherwise 0),
   stores it with `SaveProfileSetLanguage` and sets `cursor` to its index in
   `g_uiLanguageSelectLangIds` (the language value itself when not listed, e.g. 0). Returns
   `task`. */

CoreTask *UiLanguageSelectCtor(CoreTask *task)

{
  UiLanguageSelect *self = (UiLanguageSelect *)task;
  PadState *pad;
  bool wasLow;
  GfxSpriteLayer *mem;
  GfxSpriteLayer *layer;
  GfxSprite **sprites;
  s32 language;
  s32 i;
  s32 cursor;

  CoreTaskInit(task);
  self->base.vtable = g_uiLanguageSelectVtbl;
  g_gfxDisplay->frameSkip = 0;
  pad = g_padState;
  self->pad = pad;
  pad->stickEmulatesDpad = 1;

  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = (GfxSpriteLayer *)MemAlloc(sizeof(GfxSpriteLayer), (char *)0x0, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  layer = (GfxSpriteLayer *)0x0;
  if (mem != (GfxSpriteLayer *)0x0) {
    GfxSpriteLayerCtor(mem, 0);
    layer = mem;
  }
  self->layer = layer;
  layer->sorted = 1;

  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprites = (GfxSprite **)MemAlloc(27 * sizeof(GfxSprite *), (char *)0x0, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  self->sprites = sprites;

  self->state = 0;
  self->subStep = 0;
  self->pack = (IoLzsPackage *)0x0;
  self->cursor = 0;
  self->frameCount = 0;
  self->paneTex = (void *)0x0;
  self->paneOnTex = (void *)0x0;
  for (i = 0; i < 7; i++) {
    self->itemScale[i] = 1.0f;
  }
  self->done = 0;
  self->word5c = 0;
  UiLanguageSelectSetResult(self, 0);

  if (SaveHasProfile()) {
    language = PadGetLanguage(g_padState);
    switch (language) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 12:
      break;
    default:
      language = 0;
      break;
    }
    if (language < 13) {
      SaveProfileSetLanguage(SaveGetProfile(), language);
      cursor = language;
      for (i = 0; i < 7; i++) {
        if (g_uiLanguageSelectLangIds[i] == language) {
          cursor = i;
          break;
        }
      }
      self->cursor = cursor;
    }
    else {
      /* unreachable: every value reaching here is < 13 */
      SaveProfileSetLanguage(SaveGetProfile(), 0);
      self->cursor = 0;
    }
  }
  return task;
}
