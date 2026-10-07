// bdc 0x089edef4 GfxFaderStart
#include "bdc.h"

/* Starts a fade on `fader` lasting `frames` frames: sets duration `+0x4`, resets elapsed `+0x8`,
   activates it (`GfxFaderSetEnabled(f, 1)` → vtable slot 1, which sets bit 0 of the overlay rect's
   flags `+0x20` via `GfxRectSetVisible`), clears the finished flag `+0x1` and calls vtable slot 2 (an
   empty `jr ra` stub at `0x08a32498` in the fader class `0x08af577c`). */

void GfxFaderStart(GfxFader *self, s32 frames)

{
  self->duration = frames;
  self->elapsed = 0;
  GfxFaderSetEnabled(self,'\x01');
  self->done = '\0';
  ((void (*)(void *))self->vtbl[3].fn)((u8 *)self + self->vtbl[3].delta);
  return;
}

