// bdc 0x088c886c GameFieldCameraMode9Begin
#include "bdc.h"

/* Starts the field camera's mode-9 helper (`cam+0x3c0`, pointer to a `GameFieldCameraMode9State`).
   `view` holds three vectors: the object position, the current eye and the current look-at. The
   horizontal direction object → player (unit length, x/y/z clamped to [-1, 1]; (1,0,0) when its
   squared length is below 1e-5) is turned by a Y rotation of π/2 and scaled by -12, then the
   unrotated direction × 30 is added: that is `offset` (w = 0). Sets `lookTarget` = object,
   `eyeTarget` = player + offset raised by 10 (w from the player position), `eye`/`look` = the
   current view, and zeroes both velocities. */

void GameFieldCameraMode9Begin(void **holder, float *view)
{
  GameFieldCameraMode9State **h = (GameFieldCameraMode9State **)holder;
  Actor *player;
  float dot;
  float r;
  float c;
  float sn;
  float dir[3];
  float rot[3];
  float mtx[3][4];
  int i;

  player = (Actor *)ActorFindPlayer();
  /* dir = player pos - object pos, flattened */
  for (i = 0; i < 3; i++) {
    dir[i] = player->base.pos[i] - view[i];
  }
  dir[1] = 0.0f;
  dot = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
  if (dot < 1e-05f) {
    dir[2] = 0.0f;
    dir[0] = 1.0f;
  } else {
    /* normalise, x/y/z clamped to [-1, 1]; factor 0 (bank S713) for a zero length */
    dot = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
    if (dot == 0.0f) {
      r = 0.0f;
    } else {
      r = VfRsq(dot);
    }
    for (i = 0; i < 3; i++) {
      dir[i] = VfSat1(dir[i] * r);
    }
  }
  /* Y rotation by pi/2 (pi/2 * bank S703 = 2/pi quarter turns), fields x, y, z as columns */
  c = __builtin_cosf(1.57079637f);
  sn = __builtin_sinf(1.57079637f);
  mtx[0][0] = c;
  mtx[0][1] = 0.0f;
  mtx[0][2] = -sn;
  mtx[0][3] = 0.0f;
  mtx[1][0] = 0.0f;
  mtx[1][1] = 1.0f;
  mtx[1][2] = 0.0f;
  mtx[1][3] = 0.0f;
  mtx[2][0] = sn;
  mtx[2][1] = 0.0f;
  mtx[2][2] = c;
  mtx[2][3] = 0.0f;
  for (i = 0; i < 3; i++) {
    rot[i] = mtx[0][i] * dir[0] + mtx[1][i] * dir[1] + mtx[2][i] * dir[2];
  }
  /* offset = rot * -12 (w = bank S713 = 0), then offset += dir * 30 */
  for (i = 0; i < 3; i++) {
    (*h)->offset[i] = rot[i] * -12.0f;
  }
  (*h)->offset[3] = 0.0f;
  for (i = 0; i < 3; i++) {
    (*h)->offset[i] = (*h)->offset[i] + dir[i] * 30.0f;
  }
  for (i = 0; i < 4; i++) {
    (*h)->lookTarget[i] = view[i];
  }
  /* eyeTarget = player pos + offset (w from the player pos), raised by 10 */
  for (i = 0; i < 3; i++) {
    (*h)->eyeTarget[i] = player->base.pos[i] + (*h)->offset[i];
  }
  (*h)->eyeTarget[3] = player->base.pos[3];
  (*h)->eyeTarget[1] = (*h)->eyeTarget[1] + 10.0f;
  for (i = 0; i < 4; i++) {
    (*h)->eye[i] = view[4 + i];
  }
  for (i = 0; i < 4; i++) {
    (*h)->look[i] = view[8 + i];
  }
  for (i = 0; i < 4; i++) {
    (*h)->lookVel[i] = 0.0f;
  }
  for (i = 0; i < 4; i++) {
    (*h)->eyeVel[i] = 0.0f;
  }
}
