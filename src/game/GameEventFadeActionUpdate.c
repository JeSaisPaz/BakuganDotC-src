// bdc 0x088f29a8 GameEventFadeActionUpdate
#include "bdc.h"

/* Update (slot 4) of the screen-fade action: advances the frame `+9` and interpolates byte 0 of the
   fade record from byte 2 to byte 1; returns true when the duration `+8` is reached. */

bool GameEventFadeActionUpdate(GameEventFadeAction *self)

{
  GameEventFadeRecord *rec;
  int delta;

  rec = self->rec;
  self->frame = self->frame + 1;
  delta = (s16)(rec->target - rec->start);
  rec->cur = rec->start + (delta * (int)self->frame) / (int)self->frames;
  return self->frames <= self->frame;
}
