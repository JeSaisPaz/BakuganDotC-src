// bdc 0x089ff080 UiWindowFrameStepOpen
#include "bdc.h"

/* Open-animation step of a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `g_uiWindowFrameVtable` at `+0x74`, plus a `CoreObject`
   at `+0x80`) (state 1): advances the progress `+0x104` by the step and re-applies it (virtual
   `+0x54`); once alpha `+0xfc` reaches 1.0, fixes progress at 1.0, clears the step and the
   animating flag and returns to state 0. */

void UiWindowFrameStepOpen(UiWindowFrame *self)

{
  const VtblEntry *fn;

  self->progress = self->progress + self->step;
  fn = &((GfxSpriteLayer *)self)->vtbl[10];
  ((void (*)(void *, float))fn->fn)((u8 *)self + fn->delta, self->step);
  if (!(self->alpha < 1.0f)) {
    fn = &((GfxSpriteLayer *)self)->vtbl[9];
    self->progress = 1.0f;
    ((void (*)(void *, float))fn->fn)((u8 *)self + fn->delta, 1.0f);
    self->step = 0.0f;
    self->flags = self->flags & 0xfffffffd;
    self->state = 0;
  }
}
