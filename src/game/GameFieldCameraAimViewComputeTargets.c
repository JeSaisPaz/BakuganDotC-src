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
  float tx;
  float tz;
  float dirX;
  float dirY;
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
  c = VfCosQuarter(VfMul(heading, 0.636619747f));
  s = VfSinQuarter(VfMul(heading, 0.636619747f));
  /* translation of the inverse: -(rows of the rotation . its zero translation) (vtfm3.t, vneg.t) */
  tx = VfDot2(c, -s, 0.0f, 0.0f);
  tz = VfDot2(s, c, 0.0f, 0.0f);
  dirX = VfDot4(self->dir[0], self->dir[1], self->dir[2], self->dir[3], c, 0.0f, -s, -tx);
  dirY = VfDot4(self->dir[0], self->dir[1], self->dir[2], self->dir[3], 0.0f, 1.0f, 0.0f, -0.0f);
  dirZ = VfDot4(self->dir[0], self->dir[1], self->dir[2], self->dir[3], s, 0.0f, c, -tz);
  /* forward = view * (-1, 1, 0.5, 0), normalised (length factor 0 for a zero vector) */
  fx = VfDot3(-1.0f, 0.5f, 0.0f, c, -s, -tx);
  fy = 1.0f;
  fz = VfDot3(-1.0f, 0.5f, 0.0f, s, c, -tz);
  len2 = VfDot3(fx, fy, fz, fx, fy, fz);
  k = VfRsq(len2);
  if (VfIsZero(len2)) {
    k = 0.0f;
  }
  fx = VfSat1(VfMul(fx, k));
  fy = VfSat1(VfMul(fy, k));
  fz = VfSat1(VfMul(fz, k));
  /* lookAt = player pos + forward * 24 * 0.8 */
  self->lookAt[0] = VfAdd(pos[0], VfMul(VfMul(fx, 24.0f), 0.8f));
  self->lookAt[1] = VfAdd(pos[1], VfMul(VfMul(fy, 24.0f), 0.8f));
  self->lookAt[2] = VfAdd(pos[2], VfMul(VfMul(fz, 24.0f), 0.8f));
  self->lookAt[3] = pos[3];
  if (player->lockTarget == NULL) {
    self->eye[0] = pos[0];
    self->eye[1] = pos[1];
    self->eye[2] = pos[2];
    self->eye[3] = pos[3];
    self->eye[1] = self->lookAt[1];
    /* eye += dirView * 24 */
    self->eye[0] = VfAdd(self->eye[0], VfMul(dirX, 24.0f));
    self->eye[1] = VfAdd(self->eye[1], VfMul(dirY, 24.0f));
    self->eye[2] = VfAdd(self->eye[2], VfMul(dirZ, 24.0f));
  } else {
    self->eye[0] = player->lockPoint[0];
    self->eye[1] = player->lockPoint[1];
    self->eye[2] = player->lockPoint[2];
    self->eye[3] = player->lockPoint[3];
  }
}
