// bdc 0x088a27a0 ActorStageObjLandmarkTestOcclusion
#include "bdc.h"

/* Tests whether the landmark hides the unit `unit` (a `BtlBakugan`) from the camera. Returns 0
   when `unit` is NULL or neither of its virtuals 10/12 (vtable `+0x50`/`+0x60`) reports it active.
   Otherwise builds the camera-eye → unit-centre segment (unit position raised by half its stat
   height) in `g_collisionSegmentDesc` and, when the landmark has a collision mesh, tests it with
   `CollisionBoxTestSegmentQuery`: on a hit lowers `*alpha` by 0.1 (min 0.5), clears `*state`
   if it was 1 and returns 1; on a miss sets `*state = 1` (fade back in) when `*alpha < 1` and
   `*state` was 0, and returns 0. Also returns 0 without a collision mesh. */

int ActorStageObjLandmarkTestOcclusion(ActorStageObjLandmark *self, void *unit, float *alpha, char *state)

{
  BtlBakugan *bakugan = (BtlBakugan *)unit;
  const VtblEntry *vt;
  float eye[4];
  float target[4];
  float tmp[4];
  float delta[4];
  float a;
  int hidden = 0;

  if (bakugan == NULL) {
    return 0;
  }
  vt = &((const VtblEntry *)bakugan->base.base.vtable)[10];
  if (((int (*)(void *))vt->fn)((u8 *)bakugan + vt->delta) == 0) {
    vt = &((const VtblEntry *)bakugan->base.base.vtable)[12];
    if (((int (*)(void *))vt->fn)((u8 *)bakugan + vt->delta) == 0) {
      return 0;
    }
  }
  eye[0] = g_gfxActiveCamera->eye[0];
  eye[1] = g_gfxActiveCamera->eye[1];
  eye[2] = g_gfxActiveCamera->eye[2];
  eye[3] = g_gfxActiveCamera->eye[3];
  target[0] = bakugan->base.pos[0];
  target[1] = bakugan->base.pos[1];
  target[2] = bakugan->base.pos[2];
  target[3] = bakugan->base.pos[3];
  target[1] = target[1] + bakugan->combat.stats->height * 0.5f;
  /* delta = target - eye (xyz; w keeps target's w), staged through tmp as in the binary. */
  tmp[0] = target[0] - eye[0];
  tmp[1] = target[1] - eye[1];
  tmp[2] = target[2] - eye[2];
  tmp[3] = target[3];
  delta[0] = tmp[0];
  delta[1] = tmp[1];
  delta[2] = tmp[2];
  delta[3] = tmp[3];
  if (self->collisionMesh != NULL) {
    g_collisionSegmentDesc.start[0] = eye[0];
    g_collisionSegmentDesc.start[1] = eye[1];
    g_collisionSegmentDesc.start[2] = eye[2];
    g_collisionSegmentDesc.start[3] = eye[3];
    g_collisionSegmentDesc.dir[0] = delta[0];
    g_collisionSegmentDesc.dir[1] = delta[1];
    g_collisionSegmentDesc.dir[2] = delta[2];
    g_collisionSegmentDesc.dir[3] = delta[3];
    if (CollisionBoxTestSegmentQuery(self->collisionMesh, &g_collisionSegmentDesc)) {
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
  }
  return hidden;
}
