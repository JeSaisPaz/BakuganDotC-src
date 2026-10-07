// bdc 0x088f39f4 GameEventPropRecordFinish
#include "bdc.h"

/* Jumps a prop record to its end state for `kind` (0 position, 1 angles; other kinds leave the record)
   and, when `apply` and the record has a prop, pushes the current state to it: kinds 0 and 3 position
   (`GameEventPropSetPos`), 1 angles (`GameEventPropSetRot`), 2 scale (`GameEventPropSetScale`). */

void GameEventPropRecordFinish(GameEventPropRecord *self, s32 kind, u8 apply)
{
  void *prop;

  if (kind == 0) {
    self->startPos[0] = self->endPos[0];
    self->startPos[1] = self->endPos[1];
    self->startPos[2] = self->endPos[2];
    self->pos[0] = self->endPos[0];
    self->pos[1] = self->endPos[1];
    self->pos[2] = self->endPos[2];
  } else if (kind == 1) {
    self->startRot[0] = self->endRot[0];
    self->startRot[1] = self->endRot[1];
    self->startRot[2] = self->endRot[2];
    self->rot[0] = self->endRot[0];
    self->rot[1] = self->endRot[1];
    self->rot[2] = self->endRot[2];
  }
  if (apply == 0) {
    return;
  }
  prop = self->prop;
  if (prop == NULL) {
    return;
  }
  switch (kind) {
  case 0:
  case 3:
    GameEventPropSetPos(prop, self->pos);
    break;
  case 1:
    GameEventPropSetRot(prop, self->rot);
    break;
  case 2:
    GameEventPropSetScale(prop, self->scale);
    break;
  }
}
