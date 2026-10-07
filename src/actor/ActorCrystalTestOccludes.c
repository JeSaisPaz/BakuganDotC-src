// bdc 0x08857230 ActorCrystalTestOccludes
#include "bdc.h"

/* Tests whether the crystal's collision box (`collisionBox`) blocks the line from the camera eye to
   `unit`'s centre (its position raised by half its height): fills `g_collisionSegmentDesc` (start = eye,
   dir = centre - eye) and runs `CollisionBoxTestSegmentQuery`. When blocked returns 1, lowers
   `*alpha` by 0.1 (not below 0.5) and clears `*fadeIn` if it was 1; when clear returns 0 and sets
   `*fadeIn` if `*alpha` < 1 and it was 0. Returns 0 without testing when `unit` is NULL, when
   neither of its virtual slots 10/12 (`+0x50`/`+0x60`) reports true, or when the crystal has no
   collision box. */

int ActorCrystalTestOccludes(ActorCrystal *self, BtlBakugan *unit, float *alpha, u8 *fadeIn)
{
  const VtblEntry *slot;
  float eye[4];
  float centre[4];
  float dir[4];
  int occluded;

  occluded = 0;
  if (unit == NULL) {
    return 0;
  }
  slot = &((const VtblEntry *)unit->base.base.vtable)[10];
  if (((int (*)(void *))slot->fn)((u8 *)unit + slot->delta) == 0) {
    slot = &((const VtblEntry *)unit->base.base.vtable)[12];
    if (((int (*)(void *))slot->fn)((u8 *)unit + slot->delta) == 0) {
      return 0;
    }
  }
  eye[0] = g_gfxActiveCamera->eye[0];
  eye[1] = g_gfxActiveCamera->eye[1];
  eye[2] = g_gfxActiveCamera->eye[2];
  eye[3] = g_gfxActiveCamera->eye[3];
  centre[0] = unit->base.pos[0];
  centre[1] = unit->base.pos[1];
  centre[2] = unit->base.pos[2];
  centre[3] = unit->base.pos[3];
  centre[1] = centre[1] + unit->combat.stats->height * 0.5f;
  /* vsub.t: lane 3 keeps centre's w */
  dir[0] = centre[0] - eye[0];
  dir[1] = centre[1] - eye[1];
  dir[2] = centre[2] - eye[2];
  dir[3] = centre[3];
  if (self->collisionBox != NULL) {
    g_collisionSegmentDesc.start[0] = eye[0];
    g_collisionSegmentDesc.start[1] = eye[1];
    g_collisionSegmentDesc.start[2] = eye[2];
    g_collisionSegmentDesc.start[3] = eye[3];
    g_collisionSegmentDesc.dir[0] = dir[0];
    g_collisionSegmentDesc.dir[1] = dir[1];
    g_collisionSegmentDesc.dir[2] = dir[2];
    g_collisionSegmentDesc.dir[3] = dir[3];
    if (CollisionBoxTestSegmentQuery(self->collisionBox, &g_collisionSegmentDesc)) {
      float lowered = *alpha - 0.1f;

      occluded = 1;
      *alpha = lowered;
      if (lowered < 0.5f) {
        *alpha = 0.5f;
      }
      if (*fadeIn == 1) {
        *fadeIn = 0;
      }
    } else if (*alpha < 1.0f && *fadeIn == 0) {
      *fadeIn = 1;
    }
  }
  return occluded;
}
