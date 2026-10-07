// bdc 0x088c95ec UiTalkBalloonDtor
#include "bdc.h"

/* Destructor (vtable `g_uiTalkBalloonVtbl` slot 1) of the talk balloon (`UiTalkBalloonCtor`):
   re-installs its vtable, waits for the GE, frees the part sprite array `parts`, deletes the part
   layer `partLayer` (virtual dtor, flags 3), frees `buffer`, deletes the text printer, resets the
   portrait/frame-style/text-colour globals to their defaults, chains to `CoreTaskDestroy` and
   frees the task when `flags & 1`. */

void UiTalkBalloonDtor(UiTalkBalloon *self, u32 flags)
{
  const VtblEntry *dtor;

  if (self == NULL) {
    return;
  }
  self->base.vtable = g_uiTalkBalloonVtbl;
  GfxWaitGeIdle(); /* the asm also loads g_gfxDisplay into a0; GfxWaitGeIdle ignores it */
  if (self->parts != NULL) {
    MemLock();
    MemFree(self->parts, NULL, 0);
    MemUnlock();
    self->parts = NULL;
  }
  if (self->partLayer != NULL) {
    dtor = &self->partLayer->vtbl[1];
    ((void (*)(void *, int))dtor->fn)((u8 *)self->partLayer + dtor->delta, 3);
    self->partLayer = NULL;
  }
  if (self->buffer != NULL) {
    MemLock();
    MemFree(self->buffer, NULL, 0);
    MemUnlock();
    self->buffer = NULL;
  }
  if (self->printer != NULL) {
    dtor = &self->printer->layer.vtbl[1];
    ((void (*)(void *, int))dtor->fn)((u8 *)self->printer + dtor->delta, 3);
    self->printer = NULL;
  }
  g_uiTalkBalloonPortraitEnabled = 0;
  g_uiTalkBalloonFrameStyle = 1;
  g_uiTalkBalloonTextColor = 0;
  g_uiTalkBalloonTextColorParam = 0;
  CoreTaskDestroy(&self->base, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(self, NULL, 0);
    MemUnlock();
  }
}
