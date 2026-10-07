// bdc 0x088f0dbc GameEventOp7CFaceTalkPartner
#include "bdc.h"

/* Handler of event opcode 0x7c (`GameEvent470ExecCommand`): starts a talk turn (`turnPending = 1`,
   remembering talk actor `b`'s current heading in `turnHeading`), computes the heading `h` of the
   vector from `b` (`talkB`) to `a` (`talkA`) in the xz plane with `Atan2Fixed16`, then, unless
   `flag == 1`, turns `b` to `h - 0x4000` and, unless `flag == 2`, turns `a` to `h + 0x4000`, each
   with a rotate tween of `arg` frames; see `GameEvent470UpdateActorTurn`. */

void GameEventOp7CFaceTalkPartner(GameEvent470 *self, u8 flag, s16 arg)
{
  GameEventActorRecord *actors;
  GameFieldPlacedChar *ch;
  s32 a[3];
  s32 b[3];
  u16 heading;

  actors = self->actors;
  self->turnPending = 1;
  ch = actors[self->talkB].actor;
  self->turnHeading = ch->rot[1];
  a[0] = actors[self->talkA].pos[0];
  a[1] = actors[self->talkA].pos[1];
  a[2] = actors[self->talkA].pos[2];
  b[0] = actors[self->talkB].pos[0];
  b[2] = actors[self->talkB].pos[2];
  a[1] = 0;
  b[1] = 0;
  heading = (u16)(Atan2Fixed16(a[2] - b[2], -(a[0] - b[0])) + 0x4000);
  if (flag != 1) {
    self->actors[self->talkB].endRot[1] = (s16)(heading + 0x8000);
    GameEvent470AddActorTween(self, (u16)arg, 1, self->talkB);
  }
  if (flag != 2) {
    self->actors[self->talkA].endRot[1] = (s16)heading;
    GameEvent470AddActorTween(self, (u16)arg, 1, self->talkA);
  }
}
