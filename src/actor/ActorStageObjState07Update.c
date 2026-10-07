// bdc 0x088aebfc ActorStageObjState07Update
#include "bdc.h"

/* State 7 handler of the shared stage-object state machine (state `+0x304`, `MemberFnPtr` table
   `0x08a842f8`, run by `ActorStageObjUpdate`): topple-and-sink, sub-step `step`.
   Step 0 takes the fall direction: with a hitter (`hitBy`) the unit vector (cos, 0, sin, 0) of its
   yaw (`rot[1]`, also written over the hitter's `pos`), otherwise `pos - hitPos`; y is dropped,
   it is normalised and turned 90 degrees about y into `toppleAxis`; the topple values are reset
   and `pos` is taken from the model's root matrix translation. Step 1 plays sound 0x2000e7 once
   `val260` reaches 0.0628, shakes x/z (`ActorStageObjGetShakeOffset`) while `toppleTime` < 0.2,
   advances `toppleTime` by 1/240 and eases the topple angle `val260` towards pi/2 (step+1 when
   reached), sinks `sinkY` by the eased half bounding-box height (`ActorStageObjGetBounds`) and
   writes the quaternion rotation (axis, angle; translation pos.x, sinkY, pos.z) combined with the
   yaw (`rot[1]`, translation y = pos.y) into the model's root matrix, then flags the collider's
   attach matrix dirty. Step 2 plays sound 0x2000e6, spawns 12 dust effects 0xd
   (`GfxEffectSpawnDirected`) at random heights along the model (`MathMtx4TransformPoint`)
   pointing horizontally away from it, and waits 60 frames. Step 3 counts the wait down, step 4
   blinks `fade` until `timer` reaches 32 then makes the materials translucent
   (`ActorStageObjMaterialSetTranslucent`), step 5 fades out by 0.05 per frame, step 6 sets
   `removeRequest`. */

/* xyz normalised (zero length gives a zero vector), each lane clamped to [-1, 1]; w becomes 0 */
static void NormalizeXyzClamp(float *v)
{
  float len2;
  float k;

  len2 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
  if (len2 == 0.0f) {
    k = 0.0f;
  } else {
    k = VfRsq(len2);
  }
  v[0] = VfSat1(v[0] * k);
  v[1] = VfSat1(v[1] * k);
  v[2] = VfSat1(v[2] * k);
  v[3] = 0.0f;
}

void ActorStageObjState07Update(ActorStageObjBase *self)
{
  float world[16];
  float quat[4];
  float qa[4][4];
  float qb[4][4];
  float rotMtx[16];
  float yawMtx[16];
  float yawRot[4][4];
  float spawnPos[4];
  float dir[4];
  ScePspFVector4 out;
  ScePspFVector4 local;
  GfxModel *hitter;
  SndListener *listener;
  float *mtx;
  float *bounds;
  float *root;
  float angle;
  float half;
  float s;
  float c;
  float shakeX;
  float shakeZ;
  float sink;
  float ease;
  float hi;
  float h;
  float y;
  float fade;
  int off;
  int i;
  int j;
  int r;

  switch (self->step) {
  case 0:
    if (self->hitBy != NULL) {
      hitter = &((BtlBakugan *)self->hitBy)->base;
      angle = hitter->rot[1];
      /* the heading vector is built in the hitter's pos, then copied to toppleAxis */
      hitter->pos[0] = __builtin_cosf(angle);
      hitter->pos[1] = 0.0f;
      hitter->pos[2] = __builtin_sinf(angle);
      hitter->pos[3] = 0.0f;
      self->toppleAxis[0] = hitter->pos[0];
      self->toppleAxis[1] = hitter->pos[1];
      self->toppleAxis[2] = hitter->pos[2];
      self->toppleAxis[3] = hitter->pos[3];
    } else {
      self->toppleAxis[0] = self->base.pos[0] - self->hitPos[0];
      self->toppleAxis[1] = self->base.pos[1] - self->hitPos[1];
      self->toppleAxis[2] = self->base.pos[2] - self->hitPos[2];
      self->toppleAxis[3] = self->base.pos[3];
    }
    self->toppleAxis[1] = 0.0f;
    NormalizeXyzClamp(self->toppleAxis);
    /* turn 90 degrees about y: (z, y, -x) */
    c = self->toppleAxis[0];
    self->toppleAxis[0] = self->toppleAxis[2];
    self->toppleAxis[2] = -c;
    self->val260 = 0.0f;
    self->toppleTime = 0.0f;
    self->sinkY = 0.0f;
    self->toppleFlag = 0;
    root = self->base.data->rootMatrix;
    self->base.pos[0] = root[12];
    self->base.pos[1] = root[13];
    self->base.pos[2] = root[14];
    self->base.pos[3] = root[15];
    self->matrixDirty = 0;
    self->step = self->step + 1;
    break;
  case 1:
    for (i = 0; i < 16; i++) {
      yawMtx[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
    if (self->toppleFlag == 0 && !(self->val260 < 0.06283186f)) {
      self->toppleFlag = 1;
      if (SndHasListener()) {
        listener = SndGetListener();
        SndEmitterCreateAtPos(listener, 0x2000e7, &self->base.data->rootMatrix[12], 0, 1);
      }
    }
    if (self->toppleTime < 0.2f) {
      shakeX = self->shakeX;
      off = ActorStageObjGetShakeOffset(self, self->shakePhase & 0x1f);
      shakeZ = self->shakeZ;
      self->base.pos[0] = shakeX + (float)off;
      off = ActorStageObjGetShakeOffset(self, (self->shakePhase + 8) & 0x1f);
      self->shakePhase = self->shakePhase + 1;
      self->base.pos[2] = shakeZ + (float)off;
    }
    self->toppleTime = self->toppleTime + 0.004166667f;
    angle = self->val260;
    c = __builtin_cosf(self->toppleTime * self->toppleTime * 3.14159274f);
    angle = angle + (1.0f - c) * 0.5f * 1.5707964f;
    self->val260 = angle;
    if (!(angle < 1.5707964f)) {
      self->val260 = 1.5707964f;
      self->step = self->step + 1;
    }
    /* quaternion (axis * sin(a/2), cos(a/2)) */
    half = 0.318309873f * self->val260;
    c = VfCosQuarter(half);
    s = VfSinQuarter(half);
    quat[0] = self->toppleAxis[0] * s;
    quat[1] = self->toppleAxis[1] * s;
    quat[2] = self->toppleAxis[2] * s;
    quat[3] = c;
    /* rotation matrix = A . B^T with the columns of A and B swizzled from the quaternion */
    qa[0][0] = quat[3];  qa[0][1] = quat[2];  qa[0][2] = -quat[1]; qa[0][3] = -quat[0];
    qa[1][0] = -quat[2]; qa[1][1] = quat[3];  qa[1][2] = quat[0];  qa[1][3] = -quat[1];
    qa[2][0] = quat[1];  qa[2][1] = -quat[0]; qa[2][2] = quat[3];  qa[2][3] = -quat[2];
    qa[3][0] = quat[0];  qa[3][1] = quat[1];  qa[3][2] = quat[2];  qa[3][3] = quat[3];
    qb[0][0] = quat[3];  qb[0][1] = quat[2];  qb[0][2] = -quat[1]; qb[0][3] = quat[0];
    qb[1][0] = -quat[2]; qb[1][1] = quat[3];  qb[1][2] = quat[0];  qb[1][3] = quat[1];
    qb[2][0] = quat[1];  qb[2][1] = -quat[0]; qb[2][2] = quat[3];  qb[2][3] = quat[2];
    qb[3][0] = -quat[0]; qb[3][1] = -quat[1]; qb[3][2] = -quat[2]; qb[3][3] = quat[3];
    for (j = 0; j < 4; j++) {
      for (r = 0; r < 4; r++) {
        rotMtx[j * 4 + r] = qa[0][r] * qb[0][j] + qa[1][r] * qb[1][j] + qa[2][r] * qb[2][j] +
                            qa[3][r] * qb[3][j];
      }
    }
    /* w lane of the axes cleared, translation (0, 0, 0, 1) */
    rotMtx[3] = 0.0f;
    rotMtx[7] = 0.0f;
    rotMtx[11] = 0.0f;
    rotMtx[12] = 0.0f;
    rotMtx[13] = 0.0f;
    rotMtx[14] = 0.0f;
    rotMtx[15] = 1.0f;
    rotMtx[12] = self->base.pos[0];
    rotMtx[14] = self->base.pos[2];
    sink = self->sinkY;
    c = __builtin_cosf(self->toppleTime * self->toppleTime * 3.14159274f);
    ease = (1.0f - c) * 0.5f;
    hi = ActorStageObjGetBounds(self)[4];
    bounds = ActorStageObjGetBounds(self);
    sink = sink + (hi - bounds[0]) * 0.5f * ease;
    self->sinkY = sink;
    rotMtx[13] = sink;
    yawMtx[13] = self->base.pos[1];
    /* yaw rotation columns (cos, 0, -sin, 0), (0, 1, 0, 0), (sin, 0, cos, 0), (0, 0, 0, 1) */
    angle = self->base.rot[1];
    c = __builtin_cosf(angle);
    s = __builtin_sinf(angle);
    yawRot[0][0] = c;    yawRot[0][1] = 0.0f; yawRot[0][2] = -s;   yawRot[0][3] = 0.0f;
    yawRot[1][0] = 0.0f; yawRot[1][1] = 1.0f; yawRot[1][2] = 0.0f; yawRot[1][3] = 0.0f;
    yawRot[2][0] = s;    yawRot[2][1] = 0.0f; yawRot[2][2] = c;    yawRot[2][3] = 0.0f;
    yawRot[3][0] = 0.0f; yawRot[3][1] = 0.0f; yawRot[3][2] = 0.0f; yawRot[3][3] = 1.0f;
    /* vmmul.q E200,E100,E000: yawMtx <- yawRot . yawMtx^T (column-major) */
    for (i = 0; i < 4; i++) {
      for (r = 0; r < 4; r++) {
        world[i * 4 + r] = yawMtx[0 * 4 + i] * yawRot[0][r] + yawMtx[1 * 4 + i] * yawRot[1][r] +
                           yawMtx[2 * 4 + i] * yawRot[2][r] + yawMtx[3 * 4 + i] * yawRot[3][r];
      }
    }
    for (i = 0; i < 16; i++) {
      yawMtx[i] = world[i];
    }
    /* world <- rotMtx^T . yawMtx */
    for (j = 0; j < 4; j++) {
      for (r = 0; r < 4; r++) {
        world[j * 4 + r] = rotMtx[r * 4 + 0] * yawMtx[j * 4 + 0] + rotMtx[r * 4 + 1] * yawMtx[j * 4 + 1] +
                           rotMtx[r * 4 + 2] * yawMtx[j * 4 + 2] + rotMtx[r * 4 + 3] * yawMtx[j * 4 + 3];
      }
    }
    for (i = 0; i < 16; i++) {
      yawMtx[i] = world[i];
    }
    root = self->base.data->rootMatrix;
    for (i = 0; i < 16; i++) {
      root[i] = yawMtx[i];
    }
    if (self->collider != NULL) {
      ((CollisionCollider *)self->collider)->attachDirty = 1;
    }
    break;
  case 2:
    if (SndHasListener()) {
      listener = SndGetListener();
      SndEmitterCreateAtPos(listener, 0x2000e6, &self->base.data->rootMatrix[12], 0, 1);
    }
    for (i = 0; i < 12; i++) {
      mtx = self->base.data->rootMatrix;
      bounds = ActorStageObjGetBounds(self);
      h = bounds[5] * 2.0f;
      y = h * (PlatformRandFloat12() - 1.0f);
      bounds = ActorStageObjGetBounds(self);
      local.x = 0.0f;
      local.y = y - bounds[5];
      local.z = 0.0f;
      local.w = 0.0f;
      MathMtx4TransformPoint((const ScePspFMatrix4 *)mtx, &out, &local);
      spawnPos[0] = out.x;
      spawnPos[1] = out.y;
      spawnPos[2] = out.z;
      spawnPos[3] = out.w;
      dir[0] = self->base.pos[0] - spawnPos[0];
      dir[1] = self->base.pos[1] - spawnPos[1];
      dir[2] = self->base.pos[2] - spawnPos[2];
      dir[3] = self->base.pos[3];
      dir[1] = 0.0f;
      NormalizeXyzClamp(dir);
      GfxEffectSpawnDirected(g_btlUnitEffectMgr, 0xd, spawnPos, dir);
    }
    self->timer = 60;
    self->step = self->step + 1;
    break;
  case 3:
    if (self->timer == 0) {
      self->step = self->step + 1;
    } else {
      self->timer = self->timer - 1;
    }
    break;
  case 4:
    if (self->timer == 0x20) {
      GfxModelForEachMaterial(&self->base, (void *)ActorStageObjMaterialSetTranslucent, NULL);
      self->fade = 1.0f;
      self->step = self->step + 1;
    } else {
      if ((self->timer & 2) != 0) {
        fade = 1.0f;
      } else {
        fade = 0.0f;
      }
      self->fade = fade;
      self->timer = self->timer + 1;
    }
    break;
  case 5:
    fade = self->fade - 0.05f;
    self->fade = fade;
    if (fade <= 0.0f) {
      self->fade = 0.0f;
      self->step = self->step + 1;
    }
    break;
  case 6:
    self->removeRequest = 1;
    self->step = self->step + 1;
    break;
  }
}
