// bdc 0x088abba0 ActorStageObjOccludesPlayer
#include "bdc.h"

/* Occlusion test used by `ActorStageObjUpdateFade`: does the stage object hide the player unit
   `player` (a `BtlBakugan`) from the camera? Returns 0 when `player` is NULL, when neither of its
   virtuals 10/12 (vtable `+0x50`/`+0x60`) reports it active, or when the battle main task (id 100,
   `CoreTaskFind`) is in phase 9 or 6. Otherwise builds the delta from the camera eye
   (`g_gfxActiveCamera`) to the unit's centre (position raised by half its stat `height`):
   - `blocksCamera` clear: segment from the eye raised by 50 along the delta in
     `g_collisionSegmentDesc`, tested against the collision box `buffer` (0 when there is none)
     with `CollisionBoxTestSegmentQuery`;
   - `blocksCamera` set: swept sphere (`g_collisionSweptSphereDesc`, radius half the stat
     `reachRadius`) from `eye - delta` to the unit centre, tested against the mesh `collider` with
     `CollisionShapeBlockPrepare` / `CollisionMeshTestShape` (all surfaces).
   On a hit lowers `*alpha` by 0.1 (min 0.5), clears `*state` if it was 1 and returns 1; on a miss
   sets `*state = 1` (fade back in) when `*alpha < 1` and `*state` was 0, and returns 0. */

int ActorStageObjOccludesPlayer(void *unused, ActorStageObjBase *self, void *player, float *alpha, char *state)
{
  BtlBakugan *unit = (BtlBakugan *)player;
  const VtblEntry *vt;
  BtlMain *main;
  void *block;
  float delta[4];
  float eye[4];
  float target[4];
  float a;
  float r;
  int hidden = 0;

  (void)unused;
  if (unit == NULL) {
    return 0;
  }
  vt = &((const VtblEntry *)unit->base.base.vtable)[10];
  if (((int (*)(void *))vt->fn)((u8 *)unit + vt->delta) == 0) {
    vt = &((const VtblEntry *)unit->base.base.vtable)[12];
    if (((int (*)(void *))vt->fn)((u8 *)unit + vt->delta) == 0) {
      return 0;
    }
  }
  main = (BtlMain *)CoreTaskFind(100);
  if (main != NULL && (main->phase == 9 || main->phase == 6)) {
    return 0;
  }
  eye[0] = g_gfxActiveCamera->eye[0];
  eye[1] = g_gfxActiveCamera->eye[1];
  eye[2] = g_gfxActiveCamera->eye[2];
  eye[3] = g_gfxActiveCamera->eye[3];
  target[0] = unit->base.pos[0];
  target[1] = unit->base.pos[1];
  target[2] = unit->base.pos[2];
  target[3] = unit->base.pos[3];
  target[1] = target[1] + unit->combat.stats->height * 0.5f;
  /* delta = target - eye (xyz; w keeps target's w) */
  delta[0] = target[0] - eye[0];
  delta[1] = target[1] - eye[1];
  delta[2] = target[2] - eye[2];
  delta[3] = target[3];
  if (self->blocksCamera == 0) {
    eye[1] = eye[1] + 50.0f;
    g_collisionSegmentDesc.start[0] = eye[0];
    g_collisionSegmentDesc.start[1] = eye[1];
    g_collisionSegmentDesc.start[2] = eye[2];
    g_collisionSegmentDesc.start[3] = eye[3];
    g_collisionSegmentDesc.dir[0] = delta[0];
    g_collisionSegmentDesc.dir[1] = delta[1];
    g_collisionSegmentDesc.dir[2] = delta[2];
    g_collisionSegmentDesc.dir[3] = delta[3];
    if (self->buffer == NULL) {
      return hidden;
    }
    if (CollisionBoxTestSegmentQuery((CollisionBox *)self->buffer, &g_collisionSegmentDesc)) {
      hidden = 1;
      a = *alpha - 0.1f;
      *alpha = a;
      if (a < 0.5f) {
        *alpha = 0.5f;
      }
      if (*state == 1) {
        *state = 0;
      }
    } else if (*alpha < 1.0f && *state == 0) {
      *state = 1;
    }
    return hidden;
  }
  /* eye.xyz -= delta.xyz; delta = target - eye (w keeps target's w) */
  eye[0] = eye[0] - delta[0];
  eye[1] = eye[1] - delta[1];
  eye[2] = eye[2] - delta[2];
  delta[0] = target[0] - eye[0];
  delta[1] = target[1] - eye[1];
  delta[2] = target[2] - eye[2];
  delta[3] = target[3];
  g_collisionSweptSphereDesc.radius = unit->combat.stats->reachRadius * 0.5f;
  g_collisionSweptSphereDesc.start.x = eye[0];
  g_collisionSweptSphereDesc.start.y = eye[1];
  g_collisionSweptSphereDesc.start.z = eye[2];
  g_collisionSweptSphereDesc.start.w = eye[3];
  g_collisionSweptSphereDesc.dir.x = delta[0];
  g_collisionSweptSphereDesc.dir.y = delta[1];
  g_collisionSweptSphereDesc.dir.z = delta[2];
  g_collisionSweptSphereDesc.dir.w = delta[3];
  r = g_collisionSweptSphereDesc.radius;
  g_collisionSweptSphereDesc.start.w = r * r;
  /* dir.w = |dir.xyz| */
  g_collisionSweptSphereDesc.dir.w = __builtin_sqrtf(g_collisionSweptSphereDesc.dir.x * g_collisionSweptSphereDesc.dir.x +
                                                     g_collisionSweptSphereDesc.dir.y * g_collisionSweptSphereDesc.dir.y +
                                                     g_collisionSweptSphereDesc.dir.z * g_collisionSweptSphereDesc.dir.z);
  block = CollisionShapeBlockPrepare(g_collisionSweptSphereDesc.shapeBlock);
  if (CollisionMeshTestShape(((CollisionCollider *)self->collider)->shapeBlock, block, delta, 0xffffffffu) != 0) {
    hidden = 1;
    a = *alpha - 0.1f;
    *alpha = a;
    if (a < 0.5f) {
      *alpha = 0.5f;
    }
    if (*state == 1) {
      *state = 0;
    }
  } else if (*alpha < 1.0f && *state == 0) {
    *state = 1;
  }
  return hidden;
}
