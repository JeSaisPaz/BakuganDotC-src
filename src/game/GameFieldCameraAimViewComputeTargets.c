// bdc 0x088cc938 GameFieldCameraAimViewComputeTargets
#include "bdc.h"

/* Computes the targets of the field camera's throw aim helper `aim` (`cam+0x400`, a
   `GameFieldCameraAimView`). The view matrix is the inverse of a Y rotation by the player's
   heading `rot[1]`; `lookAt` (+0x20, the camera position) becomes the player position plus the
   normalised view-space (-1, 1, 0.5) direction (x/y/z clamped to [-1, 1]) scaled by 24 × 0.8, `w`
   from the player position. Without a lock-on target (`lockTarget`), `eye` (+0x10, the point
   looked at) is the player position at `lookAt`'s height moved 24 units along the view-space aim
   direction `dir`; with one, it is the locked point `lockPoint`. */

void GameFieldCameraAimViewComputeTargets(void *aim)
{
  GameFieldCameraAimView *self = (GameFieldCameraAimView *)aim;
  ActorPlayer *player;
  float *pos;
  float heading;
  float c;
  float s;
  float dirX;
  float dirZ;
  float fx;
  float fy;
  float fz;
  float len2;
  float k;

  player = (ActorPlayer *)ActorFindPlayer();
  pos = player->base.base.pos;
  heading = player->base.base.rot[1];
  /* view = transpose of the Y rotation by heading (zero translation); dirView = view * dir */
  c = __builtin_cosf(heading);
  s = __builtin_sinf(heading);
  dirX = c * self->dir[0] + -s * self->dir[2];
  dirZ = s * self->dir[0] + c * self->dir[2];
  /* forward = view * (-1, 1, 0.5, 0), normalised (length factor 0 for a zero vector) */
  fx = c * -1.0f + -s * 0.5f;
  fy = 1.0f;
  fz = s * -1.0f + c * 0.5f;
  len2 = fx * fx + fy * fy + fz * fz;
  k = VfRsq(len2);
  if (len2 == 0.0f) {
    k = 0.0f;
  }
  fx = VfSat1(fx * k);
  fy = VfSat1(fy * k);
  fz = VfSat1(fz * k);
  /* lookAt = player pos + forward * 24 * 0.8 */
  self->lookAt[0] = pos[0] + fx * 24.0f * 0.8f;
  self->lookAt[1] = pos[1] + fy * 24.0f * 0.8f;
  self->lookAt[2] = pos[2] + fz * 24.0f * 0.8f;
  self->lookAt[3] = pos[3];
  if (player->lockTarget == NULL) {
    self->eye[0] = pos[0];
    self->eye[1] = pos[1];
    self->eye[2] = pos[2];
    self->eye[3] = pos[3];
    self->eye[1] = self->lookAt[1];
    /* eye += dirView * 24 (dirView.y = dir.y) */
    self->eye[0] = self->eye[0] + dirX * 24.0f;
    self->eye[1] = self->eye[1] + self->dir[1] * 24.0f;
    self->eye[2] = self->eye[2] + dirZ * 24.0f;
  } else {
    self->eye[0] = player->lockPoint[0];
    self->eye[1] = player->lockPoint[1];
    self->eye[2] = player->lockPoint[2];
    self->eye[3] = player->lockPoint[3];
  }
}
