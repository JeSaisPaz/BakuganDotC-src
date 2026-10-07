// bdc 0x088edcac GameEventOp3EDivPropScale
#include "bdc.h"

/* Handler of event opcode 0x3e (`GameEventExecCommand`): for an existing prop and non-zero `arg`,
   divides the target scale `record+0x38` by `arg` (20.12 fixed point). */

void GameEventOp3EDivPropScale(GameEvent *self, u8 flag, s16 arg)
{
  GameEventPropRecord *rec = &self->props[flag];
  u32 a = (u16)arg;
  float f;

  if (rec->prop != NULL && a != 0) {
    f = (float)(a << 12);
    if ((s32)a > 0) {
      f = f + 0.5f;
    } else {
      f = f - 0.5f;
    }
    rec->endScale = (s32)(((long long)rec->endScale << 12) / (long long)(s32)f);
  }
}
