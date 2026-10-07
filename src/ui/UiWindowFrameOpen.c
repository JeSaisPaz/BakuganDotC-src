// bdc 0x089fe944 UiWindowFrameOpen
#include "bdc.h"

/* Starts the open animation of a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `g_uiWindowFrameVtable` at `+0x74`, plus a `CoreObject`
   at `+0x80`) (vtable `+0x24`): state 1, progress 0, applies it (virtual `+0x4c`), sets
   visible+animating flags and the per-frame step `1 / frames`; `hidden` keeps it invisible (clears
   bit 0). */

void UiWindowFrameOpen(UiWindowFrame *self, int frames, bool hidden)

{
  const VtblEntry *apply;

  self->state = 1;
  apply = &((GfxSpriteLayer *)self)->vtbl[9];
  self->progress = 0.0f;
  ((void (*)(void *))apply->fn)((u8 *)self + apply->delta);
  self->flags = self->flags | 3;
  self->step = 1.0f / (float)frames;
  if (hidden) {
    self->flags = self->flags & 0xfffffffe;
  }
}
