// bdc 0x088b3be8 StopWallUpdate
#include "bdc.h"

/* Per-frame state machine of a stop wall on `state` with countdown `timer`: 100 (set by
   `StopWallFocusCamera`) → 101 with an 80-frame countdown, `shown = 1` and sound
   `0x200260` at 55 frames left, then back to 0 with `shown` cleared; 999 (set by
   `StopWallDismiss`) → 1000 with a 30-frame countdown that every frame puts its effects into
   state 2 (`GfxEffectSetStateAttached` on `g_btlUnitEffectMgr`, attach `centre`) and plays sound
   `0x200261` at 25 frames left, then 1100, which deletes the wall through its virtual destructor
   (`StopWallDtor`, vtable entry 1, flags 3). Other states do nothing. */

void StopWallUpdate(StopWall *self)
{
  const VtblEntry *dtor;

  switch (self->state) {
  case 1100:
    if (self != NULL) {
      dtor = &((const VtblEntry *)self->base.vtable)[1];
      ((void (*)(void *, s32))dtor->fn)((u8 *)self + dtor->delta, 3);
    }
    return;
  case 999:
    self->timer = 30;
    self->state = 1000;
    /* fall through */
  case 1000:
    if (self->timer == 25 && SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x200261, 0, 0);
    }
    GfxEffectSetStateAttached(g_btlUnitEffectMgr, -1, self->centre, 2);
    self->timer = self->timer - 1;
    if (self->timer <= 0) {
      self->state = 1100;
    }
    return;
  case 100:
    self->timer = 80;
    self->shown = 1;
    self->state = 101;
    /* fall through */
  case 101:
    if (self->timer == 55 && SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x200260, 0, 0);
    }
    self->timer = self->timer - 1;
    if (self->timer <= 0) {
      self->shown = 0;
      self->state = 0;
    }
    return;
  default:
    return;
  }
}
