// bdc 0x088e6644 ActorNpcHearNoise
#include "bdc.h"

/* Noise notification (vtable slot 17, `+0x8c`) of the field NPC/guard actor classes (models
   0x4e..0x53 `npc_sm_we/mi/st`, `npc_sr_we/mi/st`; base constructor `ActorNpcCtor`, vtables
   `0x08af3b74`, `0x08af39e4`, `0x08af3d04`, `0x08af3e94`). Does nothing for the robot models
   0x51..0x53, for `level` < 1 or > 3, or (levels 1 and 3) in states 4, 6 or 11, or (level 3) when
   `pos` is not strictly inside the hearing radius `hearRadius`.
   Level 1: when `pos` is inside the radius, casts the segment query `g_collisionSegmentDesc`
   from the NPC towards `pos` (stopping 1 unit short), stores its hit point `g_collisionHitResult`
   in `noisePoint`, saves the NPC's position as `returnPoint` (unless in state 5 or 7), switches to
   state 5 and overwrites `noisePoint` with `pos`; then, like level 2, consumes a ball hit: when the
   collider `collider2` has flag 0x10 set it clears it and, for hit kind 0xbb, saves `returnPoint`
   (unless in state 5), sets state 5 and `noisePoint = pos`.
   Level 3: casts the segment from the NPC position + (10, 10, 10) by the vector to `pos`, saves
   `returnPoint` (unless in state 5 or 7), switches to state 6 and sets `noisePoint = pos`. */

void ActorNpcHearNoise(ActorNpc *self, float *pos, s32 level)
{
  float delta[4];
  float start[4];
  SegmentShape *segment;
  const VtblEntry *vt;
  CollisionCollider *collider;
  float *selfPos = self->base.base.pos;
  s32 model = (s32)self->base.base.base.unk08; /* model id */
  s32 isGuard;
  s32 i;
  float distSq;
  float len;
  float scale;
  float k;

  isGuard = 1;
  if (model >= 0x51 && model < 0x54) {
    isGuard = 0;
  }
  if (level < 2) {
    if (level <= 0) {
      return;
    }
    if (isGuard == 0 || self->aiState == 6 || self->aiState == 0xb || self->aiState == 4) {
      return;
    }
    /* vsub.t keeps lane 3 of the `pos` load. */
    delta[0] = pos[0] - selfPos[0];
    delta[1] = pos[1] - selfPos[1];
    delta[2] = pos[2] - selfPos[2];
    delta[3] = pos[3];
    distSq = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
    if (distSq < self->hearRadius * self->hearRadius) {
      segment = &g_collisionSegmentDesc;
      for (i = 0; i < 4; i++) {
        segment->start[i] = selfPos[i];
      }
      len = __builtin_sqrtf(delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2]);
      scale = len - 1.0f;
      /* Normalise (a zero length gives factor 0, S713), clamp to [-1, 1], scale; w = S713 = 0. */
      distSq = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
      k = VfRsq(distSq);
      if (distSq == 0.0f) {
        k = 0.0f;
      }
      delta[0] = VfSat1(delta[0] * k);
      delta[1] = VfSat1(delta[1] * k);
      delta[2] = VfSat1(delta[2] * k);
      segment->dir[0] = delta[0] * scale;
      segment->dir[1] = delta[1] * scale;
      segment->dir[2] = delta[2] * scale;
      segment->dir[3] = 0.0f;
      /* Slot 9 (`+0x48`) of the segment query's shape vtable. */
      vt = &((const VtblEntry *)segment->info)[9];
      ((void (*)(void *))vt->fn)((u8 *)segment + vt->delta);
      self->noisePoint[0] = g_collisionHitResult.point.x;
      self->noisePoint[1] = g_collisionHitResult.point.y;
      self->noisePoint[2] = g_collisionHitResult.point.z;
      self->noisePoint[3] = g_collisionHitResult.point.w;
      if (self->aiState != 5 && self->aiState != 7) {
        for (i = 0; i < 4; i++) {
          self->returnPoint[i] = selfPos[i];
        }
      }
      self->aiState = 5;
      self->subStep = 0;
      for (i = 0; i < 4; i++) {
        self->noisePoint[i] = pos[i];
      }
    }
  } else if (level >= 3) {
    if (level >= 4) {
      return;
    }
    if (isGuard == 0 || self->aiState == 6 || self->aiState == 0xb || self->aiState == 4) {
      return;
    }
    delta[0] = pos[0] - selfPos[0];
    delta[1] = pos[1] - selfPos[1];
    delta[2] = pos[2] - selfPos[2];
    delta[3] = pos[3];
    distSq = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
    if (!(distSq < self->hearRadius * self->hearRadius)) {
      return;
    }
    segment = &g_collisionSegmentDesc;
    /* vadd.t with (10, 10, 10, 0): lane 3 keeps the position's w. */
    start[0] = selfPos[0] + 10.0f;
    start[1] = selfPos[1] + 10.0f;
    start[2] = selfPos[2] + 10.0f;
    start[3] = selfPos[3];
    for (i = 0; i < 4; i++) {
      segment->start[i] = start[i];
    }
    for (i = 0; i < 4; i++) {
      segment->dir[i] = delta[i];
    }
    vt = &((const VtblEntry *)segment->info)[9];
    ((void (*)(void *))vt->fn)((u8 *)segment + vt->delta);
    if (self->aiState != 5 && self->aiState != 7) {
      for (i = 0; i < 4; i++) {
        self->returnPoint[i] = selfPos[i];
      }
    }
    self->aiState = 6;
    self->subStep = 0;
    for (i = 0; i < 4; i++) {
      self->noisePoint[i] = pos[i];
    }
    return;
  }

  /* Levels 1 and 2: a ball that hit the NPC's second collider. */
  if (isGuard == 0 || self->base.collider2 == NULL ||
      (((CollisionCollider *)self->base.collider2)->flags & 0x10) == 0) {
    return;
  }
  collider = (CollisionCollider *)self->base.collider2;
  collider->flags &= ~0x10u;
  if (collider->hitKind != 0xbb) {
    return;
  }
  if (self->aiState != 5) {
    for (i = 0; i < 4; i++) {
      self->returnPoint[i] = selfPos[i];
    }
  }
  self->aiState = 5;
  self->subStep = 0;
  for (i = 0; i < 4; i++) {
    self->noisePoint[i] = pos[i];
  }
}
