// bdc 0x088ebe78 GameEventOp0ASetFadeColor
#include "bdc.h"

/* Handler of event opcode 0x0a (`GameEventExecCommand`): stores the signed fade level `flag` (sign-extended
   byte) in `self->fades->target` and sets the active fader (`GfxGetActiveFader`) to fade from its current
   colour toward white (level > 0) or black (level <= 0), with end alpha `|level| / 16`. `arg` is unused. */

void GameEventOp0ASetFadeColor(GameEvent *self, u8 flag, s16 arg)
{
  GfxFader *dst;
  GfxFader *src;
  s32 level;
  float step;

  self->fades->target = (s8)flag;
  dst = GfxGetActiveFader();
  src = GfxGetActiveFader();
  dst->start[0] = src->color[0];
  dst->start[1] = src->color[1];
  dst->start[2] = src->color[2];
  dst->start[3] = src->color[3];
  level = self->fades->target;
  step = __builtin_fabsf((float)level * 0.0625f);
  if (level > 0) {
    dst = GfxGetActiveFader();
    dst->end[3] = step;
    dst->end[0] = 1.0f;
    dst->end[1] = 1.0f;
    dst->end[2] = 1.0f;
  } else {
    dst = GfxGetActiveFader();
    dst->end[3] = step;
    dst->end[0] = 0.0f;
    dst->end[1] = 0.0f;
    dst->end[2] = 0.0f;
  }
}
