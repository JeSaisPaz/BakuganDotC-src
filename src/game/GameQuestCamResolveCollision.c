// bdc 0x088fc780 GameQuestCamResolveCollision
#include "bdc.h"

/* Collision step of a quest camera spring point (a `GameQuestCamTarget`): integrates a trial step
   on copies of `point`, `vel` and `accel` (`GameQuestCamIntegrate`), pushes the trial position out
   along its direction from the controller's look-at by `ctrl->radius` (direction normalised and
   saturated to [-1,1], a zero vector getting scale 0; `g_gameQuestCamTargetAxisConsts` `posZ`
   when the squared distance is <= 0.01) and asks the controller's collision object
   (`ctrl->collision`, vtable entry 2) for a hit point (pre-zeroed) between the look-at and the
   trial position. Without a collision object or without a hit nothing is written.
   On a hit whose distance from the trial position is not below twice the radius (or NaN), or is
   <= 1e-5, `point` snaps to the hit and `vel`/`accel` become the `zero` constant; otherwise the
   unit vector toward the hit, scaled by `(dist*0.2 + 0.03) * radius * 5`, is added to `accel`
   (xyz). */

typedef struct CamCollider {
  const VtblEntry *vtbl; /* +0x00 entry 2 (+0x10): segment query, nonzero on hit */
} CamCollider;

typedef int (*CamColliderQueryFn)(float dt, void *self, float *hit, float *pos,
                                  ScePspFVector4 *from);

void GameQuestCamResolveCollision(float dt, GameQuestCamSpring *self)
{
  GameQuestCamTarget *target = (GameQuestCamTarget *)self;
  float pos[4];
  float vel[4];
  float accel[4];
  float hit[4];
  ScePspFVector4 dir;
  ScePspFVector4 diff;
  GameQuestCamCtrl *ctrl;
  CamCollider *collider;
  const VtblEntry *e;
  int hitFound;
  float lenSq;
  float scale;
  float radius;
  float dist;

  pos[0] = target->point.x;
  pos[1] = target->point.y;
  pos[2] = target->point.z;
  pos[3] = target->point.w;
  vel[0] = self->vel.x;
  vel[1] = self->vel.y;
  vel[2] = self->vel.z;
  vel[3] = self->vel.w;
  accel[0] = self->accel.x;
  accel[1] = self->accel.y;
  accel[2] = self->accel.z;
  accel[3] = self->accel.w;
  GameQuestCamIntegrate(dt, self, pos, vel, accel);

  dir.x = pos[0] - self->ctrl->lookAt.x;
  dir.y = pos[1] - self->ctrl->lookAt.y;
  dir.z = pos[2] - self->ctrl->lookAt.z;
  lenSq = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
  if (lenSq <= 0.01f) {
    dir = g_gameQuestCamTargetAxisConsts.posZ;
  } else {
    /* normalise, saturated to [-1, 1]; S713 (0) as scale for a zero vector */
    scale = VfRsq(lenSq);
    if (lenSq == 0.0f) {
      scale = 0.0f;
    }
    dir.x = VfSat1(dir.x * scale);
    dir.y = VfSat1(dir.y * scale);
    dir.z = VfSat1(dir.z * scale);
  }
  scale = self->ctrl->radius;
  dir.x = dir.x * scale;
  dir.y = dir.y * scale;
  dir.z = dir.z * scale;
  pos[0] = pos[0] + dir.x;
  pos[1] = pos[1] + dir.y;
  pos[2] = pos[2] + dir.z;

  /* C720 (0, 0, 0, 0) */
  hit[0] = 0.0f;
  hit[1] = 0.0f;
  hit[2] = 0.0f;
  hit[3] = 0.0f;
  ctrl = self->ctrl;
  if (ctrl->collision != NULL) {
    collider = (CamCollider *)ctrl->collision;
    e = &collider->vtbl[2];
    hitFound = ((CamColliderQueryFn)e->fn)(dt, (char *)collider + e->delta, hit, pos,
                                           &ctrl->lookAt);
  } else {
    hitFound = 0;
  }
  if (hitFound == 0) {
    return;
  }

  diff.x = hit[0] - pos[0];
  diff.y = hit[1] - pos[1];
  diff.z = hit[2] - pos[2];
  dist = __builtin_sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
  radius = self->ctrl->radius;
  if (dist < radius * 2.0f && !(dist <= 1e-05f)) {
    scale = 1.0f / dist;
    diff.x = diff.x * scale;
    diff.y = diff.y * scale;
    diff.z = diff.z * scale;
    scale = (dist * 0.2f + 0.030000001f) * radius * 5.0f;
    diff.x = diff.x * scale;
    diff.y = diff.y * scale;
    diff.z = diff.z * scale;
    self->accel.x = self->accel.x + diff.x;
    self->accel.y = self->accel.y + diff.y;
    self->accel.z = self->accel.z + diff.z;
    return;
  }
  target->point.x = hit[0];
  target->point.y = hit[1];
  target->point.z = hit[2];
  target->point.w = hit[3];
  self->vel = g_gameQuestCamTargetAxisConsts.zero;
  self->accel = g_gameQuestCamTargetAxisConsts.zero;
}
