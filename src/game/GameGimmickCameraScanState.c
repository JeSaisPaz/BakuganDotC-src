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
   rebuilds `coneMatrix` as the node matrix times the model root matrix (`vmmul.q M000, M100, M200`,
   row-major `node * root`), offset by (0,-1,0.9),
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
    field = (GameFieldTask *)BDC_OR_ZERO(CoreTaskFind(500));
    if (field->catchActive == 0) {
      player = (ActorPlayer *)ActorFindPlayer();
      if (player != NULL && player->base.detected == 0) {
        player->base.detected = 1;
        ActorSetState(&player->base, 9, 0);
        if (g_gameEventState.flags[5] == 0) {
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
  turns = VfMul(obj->sweepAngle, 0.318309873f);
  qYaw.x = 0.0f;
  qYaw.y = 0.0f;
  qYaw.z = VfSinQuarter(turns);
  qYaw.w = VfCosQuarter(turns);
  turns = VfMul(obj->coneAngle, 0.318309873f);
  qTilt.x = VfSinQuarter(turns);
  qTilt.y = 0.0f;
  qTilt.z = 0.0f;
  qTilt.w = VfCosQuarter(turns);

  /* qTmp = lensPos (x) qYaw; qRot = qTmp (x) qTilt (vqmul.q) */
  qTmp.x = VfDot4(obj->lensPos.x, obj->lensPos.y, obj->lensPos.z, obj->lensPos.w, qYaw.w, qYaw.z, -qYaw.y, qYaw.x);
  qTmp.y = VfDot4(-obj->lensPos.x, obj->lensPos.y, obj->lensPos.z, obj->lensPos.w, qYaw.z, qYaw.w, qYaw.x, qYaw.y);
  qTmp.z = VfDot4(obj->lensPos.x, obj->lensPos.y, obj->lensPos.z, obj->lensPos.w, qYaw.y, -qYaw.x, qYaw.w, qYaw.z);
  qTmp.w = VfDot4(-obj->lensPos.x, obj->lensPos.y, obj->lensPos.z, obj->lensPos.w, qYaw.x, -qYaw.y, -qYaw.z, qYaw.w);
  qRot.x = VfDot4(qTmp.x, qTmp.y, qTmp.z, qTmp.w, qTilt.w, qTilt.z, -qTilt.y, qTilt.x);
  qRot.y = VfDot4(-qTmp.x, qTmp.y, qTmp.z, qTmp.w, qTilt.z, qTilt.w, qTilt.x, qTilt.y);
  qRot.z = VfDot4(qTmp.x, qTmp.y, qTmp.z, qTmp.w, qTilt.y, -qTilt.x, qTilt.w, qTilt.z);
  qRot.w = VfDot4(-qTmp.x, qTmp.y, qTmp.z, qTmp.w, qTilt.x, -qTilt.y, -qTilt.z, qTilt.w);
  node = (GmoNode *)obj->lensNode;
  node->rotate[0] = qRot.x;
  node->rotate[1] = qRot.y;
  node->rotate[2] = qRot.z;
  node->rotate[3] = qRot.w;

  /* mtx = node matrix; mtx = mtx * root (row-major, vmmul.q M000, M100, M200); coneMatrix = mtx */
  node = (GmoNode *)obj->lensNode;
  for (a = 0; a < 16; a++) {
    mtx[a] = node->localMatrix[a];
  }
  root = obj->base.base.data->rootMatrix;
  for (a = 0; a < 4; a++) {
    for (b = 0; b < 4; b++) {
      prod[a * 4 + b] = VfDot4(mtx[a * 4 + 0], mtx[a * 4 + 1], mtx[a * 4 + 2], mtx[a * 4 + 3], root[0 * 4 + b], root[1 * 4 + b], root[2 * 4 + b], root[3 * 4 + b]);
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
    (&tmp.x)[b] = VfDot4(mtx[0 * 4 + b], mtx[1 * 4 + b], mtx[2 * 4 + b], mtx[3 * 4 + b], 0.0f, -1.0f, 0.9f, 0.0f);
  }
  cone->w.x = VfAdd(cone->w.x, tmp.x);
  cone->w.y = VfAdd(cone->w.y, tmp.y);
  cone->w.z = VfAdd(cone->w.z, tmp.z);

  if (obj->spotEffect != NULL) {
    /* distance along the cone axis (matrix Y) down to the camera's height */
    dist = cone->w.y - obj->base.base.pos[1];
    sinCone = VfSinQuarter(VfMul(obj->coneAngle, 0.636619747f));
    dist = -dist / sinCone;
    /* spot = coneMatrix * (0, dist, 0, 0) + coneMatrix.w.xyz */
    spot.x = VfDot4(cone->x.x, cone->y.x, cone->z.x, cone->w.x, 0.0f, dist, 0.0f, 0.0f);
    spot.y = VfDot4(cone->x.y, cone->y.y, cone->z.y, cone->w.y, 0.0f, dist, 0.0f, 0.0f);
    spot.z = VfDot4(cone->x.z, cone->y.z, cone->z.z, cone->w.z, 0.0f, dist, 0.0f, 0.0f);
    spot.w = VfDot4(cone->x.w, cone->y.w, cone->z.w, cone->w.w, 0.0f, dist, 0.0f, 0.0f);
    spot.x = VfAdd(spot.x, cone->w.x);
    spot.y = VfAdd(spot.y, cone->w.y);
    spot.z = VfAdd(spot.z, cone->w.z);
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
