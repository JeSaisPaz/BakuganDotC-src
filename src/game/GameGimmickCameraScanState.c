// bdc 0x088d85c8 GameGimmickCameraScanState
#include "bdc.h"

/* Scan state of the surveillance camera gimmick (`GameGimmickCameraCtor`, vtables
   `0x08af325c`/`0x08af3304`).
   When `GameGimmickCameraSeesPlayer` reports the player: unless the field is already catching
   (`GameFieldTask.catchActive`), catches the player once like the IR sensor (`detected`,
   `ActorSetState` 9, `ActorPlayerTakePenalty` unless `g_gameEventFlags[5]` is set,
   `waitTimer` 0, `behaviourMark` 0xff, `caughtBy` = the camera position); always switches to state 1
   with step 0.
   Otherwise runs the swing pattern `mode` (0 still; 1 sweep yaw at ±0.0179 rad/frame; 2 tilt the
   cone at ±0.00298 rad/frame; both with pauses of 3 frames between swings of 58/117 frames),
   rotates the lens node (`lensPos` * yaw(sweepAngle) * tilt(coneAngle) into `GmoNode.rotate`),
   rebuilds `coneMatrix` from the model root matrix (transposed, `vmmul.q M000, M100, M200`) times
   the node matrix, offset by (0,-1,0.9),
   moves the spot sprite to where the cone axis reaches the camera's floor height (keeping the
   sprite's Y), and while swinging (step 1) advances the angles, wrapping `sweepAngle` to ±2pi.
   Bank constant S703 (2/pi) cancels against `vsin.s`'s quarter turns: sin(coneAngle). */

void GameGimmickCameraScanState(GameGimmickCamera *obj)
{
  ScePspFVector4 qYaw;
  ScePspFVector4 qTilt;
  ScePspFVector4 qTmp;
  ScePspFVector4 qRot;
  ScePspFVector4 tmp;
  ScePspFVector4 spot;
  float mtx[16];
  float prod[16];
  const float *root;
  GameFieldTask *field;
  ActorPlayer *player;
  GmoNode *node;
  GfxModel *spotModel;
  ScePspFMatrix4 *cone;
  float dist;
  float sinCone;
  float angle;
  float turns;
  int a;
  int b;

  if (GameGimmickCameraSeesPlayer(obj) != 0) {
    field = (GameFieldTask *)CoreTaskFind(500);
    if (field->catchActive == 0) {
      player = (ActorPlayer *)ActorFindPlayer();
      if (player != NULL && player->base.detected == 0) {
        player->base.detected = 1;
        ActorSetState(&player->base, 9, 0);
        if (g_gameEventFlags[5] == 0) {
          ActorPlayerTakePenalty(player);
        }
        player->base.waitTimer = 0;
        player->behaviourMark = 0xff;
        player->caughtBy = obj->base.base.pos;
      }
    }
    obj->base.state = 1;
    obj->step = 0;
    return;
  }

  if (obj->mode == 0) {
    obj->step = 0;
    obj->coneSpeed = 0.0f;
    obj->sweepSpeed = 0.0f;
  } else if (obj->mode == 1) {
    switch (obj->step) {
    case 0:
      obj->sweepTimer = 58;
      obj->coneSpeed = 0.0f;
      obj->sweepSpeed = 0.017900813f;
      obj->sweepAngle = 0.0f;
      obj->coneAngle = 0.5235988f;
      obj->step = 1;
      break;
    case 1:
      if (obj->sweepTimer > 0) {
        obj->sweepTimer = obj->sweepTimer - 1;
      } else {
        obj->sweepTimer = 3;
        obj->step = 2;
      }
      break;
    case 2:
      if (obj->sweepTimer > 0) {
        obj->sweepTimer = obj->sweepTimer - 1;
      } else {
        obj->sweepTimer = 117;
        obj->sweepSpeed = -obj->sweepSpeed;
        obj->step = 1;
      }
      break;
    }
  } else if (obj->mode == 2) {
    switch (obj->step) {
    case 0:
      obj->sweepTimer = 58;
      obj->coneSpeed = 0.0029834688f;
      obj->sweepSpeed = 0.0f;
      obj->sweepAngle = 0.0f;
      obj->coneAngle = 0.5235988f;
      obj->step = 1;
      break;
    case 1:
      if (obj->sweepTimer > 0) {
        obj->sweepTimer = obj->sweepTimer - 1;
      } else {
        obj->sweepTimer = 3;
        obj->step = 2;
      }
      break;
    case 2:
      if (obj->sweepTimer > 0) {
        obj->sweepTimer = obj->sweepTimer - 1;
      } else {
        obj->sweepTimer = 117;
        obj->coneSpeed = -obj->coneSpeed;
        obj->step = 1;
      }
      break;
    }
  }

  /* qYaw = (0, 0, S, C) and qTilt = (S, 0, 0, C) of angle * (1/pi) quarter turns (half angles) */
  turns = obj->sweepAngle * 0.318309873f;
  qYaw.x = 0.0f;
  qYaw.y = 0.0f;
  qYaw.z = VfSinQuarter(turns);
  qYaw.w = VfCosQuarter(turns);
  turns = obj->coneAngle * 0.318309873f;
  qTilt.x = VfSinQuarter(turns);
  qTilt.y = 0.0f;
  qTilt.z = 0.0f;
  qTilt.w = VfCosQuarter(turns);

  /* qTmp = lensPos (x) qYaw; qRot = qTmp (x) qTilt (vqmul.q) */
  qTmp.x = obj->lensPos.x * qYaw.w + obj->lensPos.y * qYaw.z - obj->lensPos.z * qYaw.y +
           obj->lensPos.w * qYaw.x;
  qTmp.y = -obj->lensPos.x * qYaw.z + obj->lensPos.y * qYaw.w + obj->lensPos.z * qYaw.x +
           obj->lensPos.w * qYaw.y;
  qTmp.z = obj->lensPos.x * qYaw.y - obj->lensPos.y * qYaw.x + obj->lensPos.z * qYaw.w +
           obj->lensPos.w * qYaw.z;
  qTmp.w = -obj->lensPos.x * qYaw.x - obj->lensPos.y * qYaw.y - obj->lensPos.z * qYaw.z +
           obj->lensPos.w * qYaw.w;
  qRot.x = qTmp.x * qTilt.w + qTmp.y * qTilt.z - qTmp.z * qTilt.y + qTmp.w * qTilt.x;
  qRot.y = -qTmp.x * qTilt.z + qTmp.y * qTilt.w + qTmp.z * qTilt.x + qTmp.w * qTilt.y;
  qRot.z = qTmp.x * qTilt.y - qTmp.y * qTilt.x + qTmp.z * qTilt.w + qTmp.w * qTilt.z;
  qRot.w = -qTmp.x * qTilt.x - qTmp.y * qTilt.y - qTmp.z * qTilt.z + qTmp.w * qTilt.w;
  node = (GmoNode *)obj->lensNode;
  node->rotate[0] = qRot.x;
  node->rotate[1] = qRot.y;
  node->rotate[2] = qRot.z;
  node->rotate[3] = qRot.w;

  /* mtx = node matrix; mtx = root^T * mtx (vmmul.q M000, M100, M200); coneMatrix = mtx */
  node = (GmoNode *)obj->lensNode;
  for (a = 0; a < 16; a++) {
    mtx[a] = node->localMatrix[a];
  }
  root = obj->base.base.data->rootMatrix;
  for (a = 0; a < 4; a++) {
    for (b = 0; b < 4; b++) {
      prod[a * 4 + b] = root[b * 4 + 0] * mtx[a * 4 + 0] + root[b * 4 + 1] * mtx[a * 4 + 1] +
                        root[b * 4 + 2] * mtx[a * 4 + 2] + root[b * 4 + 3] * mtx[a * 4 + 3];
    }
  }
  for (a = 0; a < 16; a++) {
    mtx[a] = prod[a];
  }
  cone = &obj->coneMatrix;
  cone->x.x = mtx[0];
  cone->x.y = mtx[1];
  cone->x.z = mtx[2];
  cone->x.w = mtx[3];
  cone->y.x = mtx[4];
  cone->y.y = mtx[5];
  cone->y.z = mtx[6];
  cone->y.w = mtx[7];
  cone->z.x = mtx[8];
  cone->z.y = mtx[9];
  cone->z.z = mtx[10];
  cone->z.w = mtx[11];
  cone->w.x = mtx[12];
  cone->w.y = mtx[13];
  cone->w.z = mtx[14];
  cone->w.w = mtx[15];

  /* coneMatrix.w.xyz += mtx * (0, -1, 0.9, 0) (vtfm4.q E100) */
  for (b = 0; b < 3; b++) {
    (&tmp.x)[b] = mtx[0 * 4 + b] * 0.0f + mtx[1 * 4 + b] * -1.0f + mtx[2 * 4 + b] * 0.9f +
                  mtx[3 * 4 + b] * 0.0f;
  }
  cone->w.x = cone->w.x + tmp.x;
  cone->w.y = cone->w.y + tmp.y;
  cone->w.z = cone->w.z + tmp.z;

  if (obj->spotEffect != NULL) {
    /* distance along the cone axis (matrix Y) down to the camera's height */
    dist = cone->w.y - obj->base.base.pos[1];
    sinCone = __builtin_sinf(obj->coneAngle);
    dist = -dist / sinCone;
    /* spot = coneMatrix * (0, dist, 0, 0) + coneMatrix.w.xyz */
    spot.x = cone->x.x * 0.0f + cone->y.x * dist + cone->z.x * 0.0f + cone->w.x * 0.0f;
    spot.y = cone->x.y * 0.0f + cone->y.y * dist + cone->z.y * 0.0f + cone->w.y * 0.0f;
    spot.z = cone->x.z * 0.0f + cone->y.z * dist + cone->z.z * 0.0f + cone->w.z * 0.0f;
    spot.w = cone->x.w * 0.0f + cone->y.w * dist + cone->z.w * 0.0f + cone->w.w * 0.0f;
    spot.x = spot.x + cone->w.x;
    spot.y = spot.y + cone->w.y;
    spot.z = spot.z + cone->w.z;
    /* the sprite keeps its own height */
    spotModel = (GfxModel *)obj->spotEffect;
    spot.y = spotModel->ambient[1];
    spotModel->ambient[0] = spot.x;
    spotModel->ambient[1] = spot.y;
    spotModel->ambient[2] = spot.z;
    spotModel->ambient[3] = spot.w;
  }

  if (obj->step == 1) {
    obj->coneAngle = obj->coneAngle + obj->coneSpeed;
    obj->sweepAngle = obj->sweepAngle + obj->sweepSpeed;
    if (!(obj->sweepAngle <= 6.2831855f)) {
      angle = obj->sweepAngle - 6.2831855f;
      while (!(angle <= 6.2831855f)) {
        angle = angle - 6.2831855f;
      }
      obj->sweepAngle = angle;
    }
    if (obj->sweepAngle < -6.2831855f) {
      angle = obj->sweepAngle + 6.2831855f;
      while (angle < -6.2831855f) {
        angle = angle + 6.2831855f;
      }
      obj->sweepAngle = angle;
    }
  }
}
