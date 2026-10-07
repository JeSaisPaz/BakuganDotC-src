// bdc 0x089ff11c UiWindowFrameStepClose
#include "bdc.h"

/* Close-animation step of a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `g_uiWindowFrameVtable` at `+0x74`, plus a `CoreObject`
   at `+0x80`) (state 2): moves the progress `+0x104` by the step (backwards when flag 8),
   re-applies it (virtual `+0x54`); once alpha `+0xfc` reaches 0, sets progress 0, clears the
   visible and animating flags and returns to state 0. */

void UiWindowFrameStepClose(UiWindowFrame *self)

{
  const VtblEntry *fn;
  float negStep;

  negStep = -self->step;
  if ((self->flags & 8) != 0) {
    self->progress = self->progress + negStep;
  }
  else {
    self->progress = self->progress + self->step;
  }
  fn = &((GfxSpriteLayer *)self)->vtbl[10];
  ((void (*)(void *, float))fn->fn)((u8 *)self + fn->delta, negStep);
  if (self->alpha <= 0.0f) {
    fn = &((GfxSpriteLayer *)self)->vtbl[9];
    self->progress = 0.0f;
    ((void (*)(void *, float))fn->fn)((u8 *)self + fn->delta, 0.0f);
    self->step = 0.0f;
    self->flags = self->flags & 0xfffffffc;
    self->state = 0;
  }
}
