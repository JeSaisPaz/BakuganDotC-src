// bdc 0x088098c8 UiLanguageSelectDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the language-selection screen (id 199): reinstalls
   `g_uiLanguageSelectVtbl`, waits for the GE, frees the sprite array `sprites`, deletes the
   sprite pool `layer` and the `LanguageSetting.lzs` package `pack` through their virtual
   destructors (slot 1, flags 3), turns stick→D-pad emulation of `pad` off, re-enables frame skip,
   chains to `CoreTaskDestroy``(task, 0)` and frees the object when `flags & 1`. Does nothing for
   NULL. */

void UiLanguageSelectDtor(CoreTask *task, u32 flags)

{
  UiLanguageSelect *self = (UiLanguageSelect *)task;
  const VtblEntry *e;
  GfxSprite **sprites;

  if (self != (UiLanguageSelect *)0x0) {
    self->base.vtable = g_uiLanguageSelectVtbl;
    GfxWaitGeIdle();
    if (self->sprites != (GfxSprite **)0x0) {
      sprites = self->sprites;
      MemLock();
      MemFree(sprites, (char *)0x0, 0);
      MemUnlock();
      self->sprites = (GfxSprite **)0x0;
    }
    if (self->layer != (GfxSpriteLayer *)0x0) {
      e = self->layer->vtbl + 1;
      ((void (*)(void *, int))e->fn)((u8 *)self->layer + e->delta, 3);
      self->layer = (GfxSpriteLayer *)0x0;
    }
    if (self->pack != (IoLzsPackage *)0x0) {
      e = (const VtblEntry *)self->pack->base.vtable + 1;
      ((void (*)(void *, int))e->fn)((u8 *)self->pack + e->delta, 3);
      self->pack = (IoLzsPackage *)0x0;
    }
    self->pad->stickEmulatesDpad = 0;
    g_gfxDisplay->frameSkip = 1;
    CoreTaskDestroy(&self->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, (char *)0x0, 0);
      MemUnlock();
    }
  }
  return;
}
