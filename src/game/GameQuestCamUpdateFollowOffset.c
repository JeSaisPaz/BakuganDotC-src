// bdc 0x088f83c8 GameQuestCamUpdateFollowOffset
#include "bdc.h"

/* Follow offset of the point camera mode (class built by `GameQuestCamPointModeCtor`): the eye
   goal is the followed target's position (its vtable entry 2) plus `followOffset`, w lane taken from
   the target position. The direction from the spring point to the tracked entry object's position
   (`base.entry` -> object, vtable entry 2) is normalised (saturated to [-1,1], a zero vector getting
   scale 0; `g_gameQuestCamPointModeAxisConsts` `zero` when its squared length is <= 0.01). When
   the controller is not snapping (`ctrl->snap == 0`) and that direction drifts from `viewDir`
   (dot < 0.3), the eye x/z are replaced by the entry object's position plus `cross(dir, axisY)`
   scaled by `entry->sideOffset * 1.5`, negated when the cross product points along `viewDir`
   (dot >= 0 or NaN). The result is stored to `goal`, then `GameQuestCamRefreshLookAt` runs when
   `entry->collide` is set. */

typedef struct CamFollowOwner {
  void *unk0;
  const VtblEntry *vtbl; /* +0x04 entry 2 (+0x10): position getter */
} CamFollowOwner;

typedef struct CamFollowEntryObj {
  const VtblEntry *vtbl; /* +0x00 entry 2 (+0x10): position getter */
} CamFollowEntryObj;

void GameQuestCamUpdateFollowOffset(GameQuestCamPointMode *self)
{
  ScePspFVector4 eye;
  ScePspFVector4 dir;
  ScePspFVector4 side;
  const ScePspFVector4 *axis;
  CamFollowOwner *owner;
  CamFollowEntryObj *obj;
  const VtblEntry *e;
  const float *pos;
  float lenSq;
  float dot;
  float scale;

  owner = (CamFollowOwner *)self->base.base.base.followed;
  e = &owner->vtbl[2];
  pos = ((const float *(*)(void *))e->fn)((char *)owner + e->delta);
  eye.x = pos[0] + self->followOffset.x;
  eye.y = pos[1] + self->followOffset.y;
  eye.z = pos[2] + self->followOffset.z;
  eye.w = pos[3];

  obj = *(CamFollowEntryObj **)self->base.entry;
  e = &obj->vtbl[2];
  pos = ((const float *(*)(void *))e->fn)((char *)obj + e->delta);
  dir.x = pos[0] - self->base.base.point.x;
  dir.y = pos[1] - self->base.base.point.y;
  dir.z = pos[2] - self->base.base.point.z;
  lenSq = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
  if (lenSq <= 0.01f) {
    dir = g_gameQuestCamPointModeAxisConsts.zero;
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

  if (self->base.base.base.ctrl->snap == 0) {
    dot = dir.x * self->viewDir.x + dir.y * self->viewDir.y + dir.z * self->viewDir.z;
    if (dot < 0.3f) {
      axis = &g_gameQuestCamPointModeAxisConsts.axisY;
      side.x = dir.y * axis->z - dir.z * axis->y;
      side.y = dir.z * axis->x - dir.x * axis->z;
      side.z = dir.x * axis->y - dir.y * axis->x;
      dot = side.x * self->viewDir.x + side.y * self->viewDir.y + side.z * self->viewDir.z;
      if (dot < 0.0f) {
        scale = self->entry->sideOffset * 1.5f;
      } else {
        scale = self->entry->sideOffset * -1.5f;
      }
      side.x = side.x * scale;
      side.y = side.y * scale;
      side.z = side.z * scale;
      obj = *(CamFollowEntryObj **)self->base.entry;
      e = &obj->vtbl[2];
      pos = ((const float *(*)(void *))e->fn)((char *)obj + e->delta);
      eye.x = pos[0] + side.x;
      eye.z = pos[2] + side.z;
    }
  }
  self->base.base.base.goal = eye;
  if (self->entry->collide != 0) {
    /* The listing passes self in a0; the callee is an empty hook declared without parameters. */
    GameQuestCamRefreshLookAt();
  }
}
