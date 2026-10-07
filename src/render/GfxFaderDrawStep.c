// bdc 0x089edcf0 GfxFaderDrawStep
#include "bdc.h"

/* Draws an active fader through its draw method (vtable `+0x2c`, `GfxScreenFaderDraw`) and sets
   the done byte `+1` once `elapsed == duration`. */

void GfxFaderDrawStep(GfxFader *self)

{
  if (self->active != '\0') {
    ((void (*)(void *))self->vtbl[5].fn)((u8 *)self + self->vtbl[5].delta);
  }
  if ((self->done == '\0') && (self->elapsed == self->duration)) {
    self->done = '\x01';
  }
  return;
}

