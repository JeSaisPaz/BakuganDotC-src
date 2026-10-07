// bdc 0x089fe9dc UiWindowFrameClose
#include "bdc.h"

/* Starts the close animation of a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `g_uiWindowFrameVtable` at `+0x74`, plus a `CoreObject`
   at `+0x80`) (vtable `+0x2c`): state 2, progress 1.0, sets the animating flag and the step `1 /
   frames`; `reverse` sets flag 8 (progress runs backwards). */

void UiWindowFrameClose(UiWindowFrame *self, int frames, bool reverse)

{
  const VtblEntry *fn;

  self->state = 2;
  fn = &((GfxSpriteLayer *)self)->vtbl[9];
  self->progress = 1.0f;
  ((void (*)(void *, float))fn->fn)((u8 *)self + fn->delta, 1.0f);
  self->flags = self->flags | 2;
  self->step = 1.0f / (float)frames;
  if (reverse) {
    self->flags = self->flags | 8;
  }
  else {
    self->flags = self->flags & 0xfffffff7;
  }
}
