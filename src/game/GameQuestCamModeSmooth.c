// bdc 0x088fd5c4 GameQuestCamModeSmooth
#include "bdc.h"

/* Look-ahead smoothing of a quest camera mode: on the first call (`firstCall`) captures the
   followed object's position (its vtable slot `+0x10`) into `targetPos`; clears `lead`; when the
   controller's `snap` flag is set, resets `smoothVel` and `smoothPos` to the zero constant and
   returns. Otherwise, unless `behind` is clear or the current `.ptlb` node (`node`, flag `flag51`)
   disables it (both: `leadFilter = 0`, `lead = 0`), projects the target's movement since the capture
   onto `dir`, scales it by `leadScale`, low-pass filters it into `leadFilter`
   (`0.2*new + 0.8*old`, skipped when |new| <= 0.001) and sets `lead = dir * leadFilter` (w = 0).
   Finally integrates a critically damped spring velocity `smoothVel` pulling `smoothPos` toward
   `lead` (stiffness `(w/3)^2` stored in `springFactor` from `w` = `stiffness`, damping `w`, step
   `dt`) and adds it to the camera velocity `vel` (x, y, z; the w lanes of `smoothVel` and `vel` are
   kept). The VFPU bank constants it reads (C720, S713) are zero. */

typedef struct {
  void *pad;
  VtblEntry *vtbl;
} GameQuestCamFollowed;

void GameQuestCamModeSmooth(float dt, GameQuestCamModeBase *self, float *dir, float leadScale)
{
  GameQuestCamFollowed *obj;
  VtblEntry *vt;
  const ScePspFVector4 *p;
  GameQuestPathNodeVec *vec;
  GameQuestPathNode **slot;
  GameQuestPathNode *node;
  int index;
  float dx, dy, dz;
  float proj;
  float mag;
  float k;
  float stiff;
  float ax, ay, az;

  if (self->firstCall != 0) {
    obj = (GameQuestCamFollowed *)(self->base).base.followed;
    vt = obj->vtbl;
    p = ((const ScePspFVector4 *(*)(void *))vt[2].fn)((u8 *)obj + vt[2].delta);
    self->targetPos = *p;
    self->firstCall = 0;
  }
  self->lead.x = 0.0f;
  self->lead.y = 0.0f;
  self->lead.z = 0.0f;
  self->lead.w = 0.0f;
  if ((self->base).base.ctrl->snap != 0) {
    self->smoothVel = g_gameQuestCamModeBaseAxisConsts.zero;
    self->smoothPos = g_gameQuestCamModeBaseAxisConsts.zero;
    return;
  }
  node = NULL;
  if (self->node != -1) {
    index = self->node;
    vec = g_questPathSet->cur;
    if ((index >= 0) && (index < vec->count)) {
      slot = &vec->data[index];
    } else {
      memset(&g_gameQuestPathNullNode, 0, 4);
      slot = &g_gameQuestPathNullNode;
    }
    node = *slot;
  }
  if ((self->behind == 0) || ((node != NULL) && (node->flag51 != 0))) {
    self->leadFilter = 0.0f;
    self->lead.x = 0.0f;
    self->lead.y = 0.0f;
    self->lead.z = 0.0f;
    self->lead.w = 0.0f;
  } else {
    obj = (GameQuestCamFollowed *)(self->base).base.followed;
    vt = obj->vtbl;
    p = ((const ScePspFVector4 *(*)(void *))vt[2].fn)((u8 *)obj + vt[2].delta);
    /* movement since the capture, projected onto dir */
    dx = p->x - self->targetPos.x;
    dy = p->y - self->targetPos.y;
    dz = p->z - self->targetPos.z;
    proj = dx * dir[0] + dy * dir[1] + dz * dir[2];
    proj = proj * leadScale;
    mag = proj;
    if (proj < 0.0f) {
      mag = -proj;
    }
    if (!(mag <= 0.001f)) {
      self->leadFilter = proj * 0.2f + self->leadFilter * 0.8f;
    }
    k = self->leadFilter;
    self->lead.x = dir[0] * k;
    self->lead.y = dir[1] * k;
    self->lead.z = dir[2] * k;
    self->lead.w = 0.0f;
  }
  k = (self->base).base.stiffness * 0.33333334f;
  k = k * k;
  (self->base).base.springFactor = k;
  /* spring pull (lead - smoothPos) * k */
  ax = (self->lead.x - self->smoothPos.x) * k;
  ay = (self->lead.y - self->smoothPos.y) * k;
  az = (self->lead.z - self->smoothPos.z) * k;
  /* damping smoothVel * stiffness */
  stiff = (self->base).base.stiffness;
  ax = (ax - self->smoothVel.x * stiff) * dt;
  ay = (ay - self->smoothVel.y * stiff) * dt;
  az = (az - self->smoothVel.z * stiff) * dt;
  self->smoothVel.x = self->smoothVel.x + ax;
  self->smoothVel.y = self->smoothVel.y + ay;
  self->smoothVel.z = self->smoothVel.z + az;
  (self->base).base.vel.x = (self->base).base.vel.x + self->smoothVel.x;
  (self->base).base.vel.y = (self->base).base.vel.y + self->smoothVel.y;
  (self->base).base.vel.z = (self->base).base.vel.z + self->smoothVel.z;
}
