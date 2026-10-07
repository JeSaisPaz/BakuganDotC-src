// bdc 0x088f3524 GameEventActorTweenUpdate
#include "bdc.h"

/* Per-frame update (vtable `0x08af4344` slot 4, `+0x24`; class built by
   `GameEventActorTweenCtor`) of an event-actor tween: advances `frame` toward `frames` and, by
   `kind` (0 position, 1 rotation, anything else nothing), interpolates the event-actor record
   `rec` — the 20.12 position from `startPos` to `endPos` into `pos`, copied into the placed
   character `rec->actor`, or the `s16` angles `startRot` -> `endRot` into `rot`, copied into the
   character's `rot` with the heading `rot[1]` also into `work2c`. Returns 1 once `frame >= frames`,
   else 0. */

u8 GameEventActorTweenUpdate(GameEventActorTween *self)
{
  GameEventActorRecord *rec;
  GameFieldPlacedChar *ch;
  s32 t;
  s32 start0;
  s32 d0;
  s32 d1;
  s32 d2;

  self->frame++;
  if (self->kind == 0) {
    t = ((s32)self->frame << 12) / (s32)self->frames;
    rec = self->rec;
    start0 = rec->startPos[0];
    d0 = rec->endPos[0] - start0;
    d1 = rec->endPos[1] - rec->startPos[1];
    d2 = rec->endPos[2] - rec->startPos[2];
    rec->pos[0] = (s32)(((s64)t * d0) >> 12) + start0;
    rec->pos[1] = (s32)(((s64)t * d1) >> 12) + rec->startPos[1];
    rec->pos[2] = (s32)(((s64)t * d2) >> 12) + rec->startPos[2];
    rec = self->rec;
    ch = (GameFieldPlacedChar *)rec->actor;
    ch->pos[0] = rec->pos[0];
    ch->pos[1] = rec->pos[1];
    ch->pos[2] = rec->pos[2];
  } else if (self->kind == 1) {
    rec = self->rec;
    rec->rot[0] = rec->startRot[0] +
        ((s32)(s16)(rec->endRot[0] - rec->startRot[0]) * (s32)self->frame) / (s32)self->frames;
    rec = self->rec;
    rec->rot[1] = rec->startRot[1] +
        ((s32)(s16)(rec->endRot[1] - rec->startRot[1]) * (s32)self->frame) / (s32)self->frames;
    rec = self->rec;
    rec->rot[2] = rec->startRot[2] +
        ((s32)(s16)(rec->endRot[2] - rec->startRot[2]) * (s32)self->frame) / (s32)self->frames;
    rec = self->rec;
    ch = (GameFieldPlacedChar *)rec->actor;
    ch->rot[0] = rec->rot[0];
    ch->rot[1] = rec->rot[1];
    ch->rot[2] = rec->rot[2];
    rec = self->rec;
    ((GameFieldPlacedChar *)rec->actor)->work2c = rec->rot[1];
  }
  return !(self->frame < self->frames);
}
