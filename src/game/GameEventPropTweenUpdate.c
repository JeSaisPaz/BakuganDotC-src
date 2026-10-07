// bdc 0x088f3b38 GameEventPropTweenUpdate
#include "bdc.h"

/* Per-frame update (vtable `0x08af437c` slot 4, `+0x24`) of the event prop tween action
   (`GameEventPropTweenCtor`): advances `frame` and, by `kind`, interpolates the prop record
   from start to end over `frames` frames and applies it to the prop: 0 position (also latching
   `bobBaseY`, `GameEventPropSetPos`), 1 rotation (`GameEventPropSetRot`), 2 scale
   (`GameEventPropSetScale`), 3 a looping vertical bob `bobBaseY + sin * bobAmp`
   (`GameEventPropSinFixed`, frame wrapped modulo `frames`). Returns 1 at once when the record
   has no prop, otherwise 1 once `frame >= frames` (never for kind 3, whose frame wraps). */

u8 GameEventPropTweenUpdate(GameEventPropTween *self)
{
  GameEventPropRecord *rec = self->rec;
  u32 frame;
  u32 frames;
  s32 t;
  s32 d0, d1, d2;
  s32 startScale, dScale, num, den;
  s32 s;

  if (rec->prop == NULL) {
    return 1;
  }
  self->frame = self->frame + 1;
  frame = self->frame;
  frames = self->frames;
  switch (self->kind) {
  case 0:
    t = (s32)(frame << 12) / (s32)frames;
    d0 = rec->endPos[0] - rec->startPos[0];
    d1 = rec->endPos[1] - rec->startPos[1];
    d2 = rec->endPos[2] - rec->startPos[2];
    rec->pos[0] = (s32)(((s64)t * d0) >> 12) + rec->startPos[0];
    rec->pos[1] = (s32)(((s64)t * d1) >> 12) + rec->startPos[1];
    rec->pos[2] = (s32)(((s64)t * d2) >> 12) + rec->startPos[2];
    self->rec->bobBaseY = self->rec->pos[1];
    GameEventPropSetPos(self->rec->prop, self->rec->pos);
    break;
  case 1:
    rec->rot[0] = rec->startRot[0] +
                  ((s16)(rec->endRot[0] - rec->startRot[0]) * (s32)frame) / (s32)frames;
    rec = self->rec;
    rec->rot[1] = rec->startRot[1] +
                  ((s16)(rec->endRot[1] - rec->startRot[1]) * (s32)self->frame) /
                      (s32)self->frames;
    rec = self->rec;
    rec->rot[2] = rec->startRot[2] +
                  ((s16)(rec->endRot[2] - rec->startRot[2]) * (s32)self->frame) /
                      (s32)self->frames;
    GameEventPropSetRot(self->rec->prop, self->rec->rot);
    break;
  case 2:
    startScale = rec->startScale;
    dScale = rec->endScale - startScale;
    if ((s32)frame > 0) {
      t = (s32)((float)(s32)(frame << 12) + 0.5f);
    } else {
      t = (s32)((float)(s32)(frame << 12) - 0.5f);
    }
    num = (s32)(((s64)dScale * t + g_gameEventFixedRound64) >> 12);
    if ((s32)frames > 0) {
      den = (s32)((float)(s32)(frames << 12) + 0.5f);
    } else {
      den = (s32)((float)(s32)(frames << 12) - 0.5f);
    }
    rec->scale = startScale + (s32)(((s64)num << 12) / den);
    GameEventPropSetScale(self->rec->prop, self->rec->scale);
    break;
  case 3:
    self->frame = (s32)frame % (s32)frames;
    s = GameEventPropSinFixed(((s32)(self->frame << 16) / (s32)frames) & 0xffff);
    rec = self->rec;
    rec->pos[1] = rec->bobBaseY + (s32)(((s64)s * rec->bobAmp + g_gameEventFixedRound64) >> 12);
    GameEventPropSetPos(self->rec->prop, self->rec->pos);
    break;
  default:
    return !((s32)frame < (s32)frames);
  }
  return !((s32)self->frame < (s32)self->frames);
}
