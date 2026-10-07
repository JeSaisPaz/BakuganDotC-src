// bdc 0x089edb98 GfxFaderBaseCtor
#include "bdc.h"

/* Constructor of the base fader (base vtable `0x08af574c`; screen fader vtable `0x08af577c`, 0x70
   bytes: `+0x0` active, `+0x1` done, `+0x4` duration, `+0x8` elapsed, `+0xc` progress, `+0x10` sort
   key, `+0x20` current RGBA, `+0x30` start RGBA, `+0x40` end RGBA, `+0x60` overlay rect): inactive,
   zero duration/progress, sort key 0, current/start/end colours cleared. */

GfxFader * GfxFaderBaseCtor(GfxFader *self)

{
  self->vtbl = g_gfxFaderBaseVtbl;
  self->duration = 0;
  self->elapsed = 0;
  self->active = '\0';
  self->done = '\0';
  self->sortKey = 0.0f;
  self->color[0] = 0.0f;
  self->color[1] = 0.0f;
  self->color[2] = 0.0f;
  self->color[3] = 0.0f;
  self->start[0] = 0.0f;
  self->start[1] = 0.0f;
  self->start[2] = 0.0f;
  self->start[3] = 0.0f;
  self->end[0] = 0.0f;
  self->end[1] = 0.0f;
  self->end[2] = 0.0f;
  self->end[3] = 0.0f;
  return self;
}
