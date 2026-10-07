// bdc 0x088edbe0 GameEventOp3DMulPropScale
#include "bdc.h"

/* Handler of event opcode 0x3d (`GameEventExecCommand`): for an existing prop, sets the target
   scale `record+0x38` to the start scale `+0x34` times `arg` (both 20.12 fixed point,
   `__muldi3` with rounding 0x800). */

void GameEventOp3DMulPropScale(GameEvent *self, u8 flag, s16 arg)
{
  GameEventPropRecord *rec;
  float f;
  s64 r;

  rec = self->props;
  if (rec[flag].prop != (void *)0x0) {
    if ((u16)arg == 0) {
      f = (float)((u32)(u16)arg << 12) - 0.5f;
    } else {
      f = (float)((u32)(u16)arg << 12) + 0.5f;
    }
    r = (s64)rec[flag].scale * (s64)(s32)f + 0x800LL;
    rec[flag].endScale = (s32)(r >> 12);
  }
}
