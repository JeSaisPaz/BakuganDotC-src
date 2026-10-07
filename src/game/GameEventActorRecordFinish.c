// bdc 0x088f33ec GameEventActorRecordFinish
#include "bdc.h"

/* Jumps an event-actor record to its end state for `kind` (0 position, 1 angles; other kinds
   untouched): end values become both start and current values. When `apply`, copies the current
   value into the placed character `actor` (position, or angles plus the heading copy `work2c`). */

void GameEventActorRecordFinish(GameEventActorRecord *self, s32 kind, u8 apply)
{
  GameFieldPlacedChar *ch;

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
  if (apply != 0) {
    if (kind == 0) {
      ch = self->actor;
      ch->pos[0] = self->pos[0];
      ch->pos[1] = self->pos[1];
      ch->pos[2] = self->pos[2];
    } else if (kind == 1) {
      ch = self->actor;
      ch->rot[0] = self->rot[0];
      ch->rot[1] = self->rot[1];
      ch->rot[2] = self->rot[2];
      ch = self->actor;
      ch->work2c = self->rot[1];
    }
  }
}
