// bdc 0x088b1a6c ActorStageObjPropState03Topple
#include "bdc.h"

/* Column-major 4x4 product `d = a * b` (`vmmul`): column j of `d` is the sum over k of
   b[j][k] * column k of `a`, summed left to right. Both inputs are read before `d` is written. */
static void ToppleMtx4Mul(float *d, const float *a, const float *b)
{
  float r[16];
  int j;
  int i;

  for (j = 0; j < 4; j++) {
    for (i = 0; i < 4; i++) {
      r[j * 4 + i] = b[j * 4 + 0] * a[0 * 4 + i] + b[j * 4 + 1] * a[1 * 4 + i] +
                     b[j * 4 + 2] * a[2 * 4 + i] + b[j * 4 + 3] * a[3 * 4 + i];
    }
  }
  for (i = 0; i < 16; i++) {
    d[i] = r[i];
  }
}

/* Normalises v.xyz in place (`vrsq` of the squared length, factor 0 for a zero vector), each lane
   clamped to [-1, 1]; w becomes 0 (the masked lane of the bank's C710 holds S713 = 0). */
static void ToppleNormalize3(float *v)
{
  float lenSq;
  float k;

  lenSq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
  k = VfRsq(lenSq);
  if (lenSq == 0.0f) {
    k = 0.0f;
  }
  v[0] = VfSat1(v[0] * k);
  v[1] = VfSat1(v[1] * k);
  v[2] = VfSat1(v[2] * k);
  v[3] = 0.0f;
}

/* State 3 of the knock-over prop (signs, billboards, lights; sub-step `step`):
   step 0 plays the kind's hit sound, takes the push direction from the hitting unit
   (model position - `contactPos`; y always dropped, x also dropped for
   `ActorStageObjKindTopples` kinds), normalises it and turns it 90 degrees about y into
   `toppleAxis`, clamps the unit's horizontal speed to 45 (`toppleSpeed`) and resets the topple
   values, then falls into step 1. Step 1 eases `toppleAngle` towards pi/2 (step+1 when reached),
   builds the rotation quaternion about `toppleAxis` and writes
   rotation(+rise) * yaw(+pos.y) into the model's root matrix. Step 2 plays sound 0x2000e6 and waits
   30 frames, or for billboards (`ActorStageObjIsBillboard`) spawns 8 dust effects 0xd
   (`GfxEffectSpawnDirected`) at random heights along the model (`MathMtx4TransformPoint`)
   and waits 60. Step 3 counts the wait down, step 4 blinks `fade` for 32 frames then makes the
   materials translucent (`ActorStageObjPropToppleMaterialSetTranslucent`), step 5 fades out by
   0.05 per frame, step 6 calls the virtual break (vtable entry 11, `ActorStageObjPropBreak`).
   Returns nothing. */

void ActorStageObjPropState03Topple(ActorStageObjProp *self)
{
  float qa[16];
  float qb[16];
  float rotMtx[16];
  float yawRot[16];
  float yawMtx[16];
  float world[16];
  float quat[4];
  float spawnPos[4];
  float dir[4];
  float *axis;
  float *mtx;
  float *bounds;
  ScePspFVector4 out;
  ScePspFVector4 local;
  const VtblEntry *brk;
  float angle;
  float half;
  float s;
  float c;
  float t;
  float x;
  float h;
  float y;
  float fade;
  float speed;
  int i;

  switch (self->step) {
  case 0:
    ActorStageObjPropPlaySound(self, ((s32 *)self->base.soundParams)[0]);
    mtx = self->base.base.data->rootMatrix;
    axis = self->base.toppleAxis;
    /* vsub.t: w keeps the root matrix's w.w */
    axis[0] = mtx[12] - self->contactPos[0];
    axis[1] = mtx[13] - self->contactPos[1];
    axis[2] = mtx[14] - self->contactPos[2];
    axis[3] = mtx[15];
    if (ActorStageObjKindTopples(&self->base) != 0) {
      self->base.toppleAxis[0] = 0.0f;
    }
    self->base.toppleAxis[1] = 0.0f;
    ToppleNormalize3(axis);
    /* turn 90 degrees about y: (z, y, -x) */
    x = axis[0];
    axis[0] = axis[2];
    axis[2] = -x;
    speed = __builtin_sqrtf(self->contactVel[0] * self->contactVel[0] +
                            self->contactVel[2] * self->contactVel[2]);
    self->toppleSpeed = speed;
    if (!(speed <= 45.0f)) {
      self->toppleSpeed = 45.0f;
    }
    self->toppleAngle = 0.0f;
    self->toppleT = 0.0f;
    self->toppleRise = 0.0f;
    self->flag344 = 0;
    self->step = self->step + 1;
    /* fall through */
  case 1:
    for (i = 0; i < 16; i++) {
      yawMtx[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
    self->toppleT = self->toppleT + 1.0f / (50.0f - self->toppleSpeed);
    angle = self->toppleAngle;
    t = self->toppleT;
    angle = angle + (1.0f - __builtin_cosf(t * t * 3.1415927f)) * 0.5f * 1.5707964f;
    self->toppleAngle = angle;
    if (!(angle < 1.5707964f)) {
      self->toppleAngle = 1.5707964f;
      self->step = self->step + 1;
    }
    /* quaternion (axis * sin(a/2), cos(a/2)): vcos/vsin of a * (1/pi) quarter turns */
    half = 0.318309873f * self->toppleAngle;
    c = VfCosQuarter(half);
    s = VfSinQuarter(half);
    axis = self->base.toppleAxis;
    quat[0] = axis[0] * s;
    quat[1] = axis[1] * s;
    quat[2] = axis[2] * s;
    quat[3] = c;
    /* rotation matrix = L(q) * R(conj q) as one 4x4 product (vpfxs swizzles) */
    qa[0] = quat[3];   qa[1] = quat[2];   qa[2] = -quat[1];  qa[3] = -quat[0];
    qa[4] = -quat[2];  qa[5] = quat[3];   qa[6] = quat[0];   qa[7] = -quat[1];
    qa[8] = quat[1];   qa[9] = -quat[0];  qa[10] = quat[3];  qa[11] = -quat[2];
    qa[12] = quat[0];  qa[13] = quat[1];  qa[14] = quat[2];  qa[15] = quat[3];
    qb[0] = quat[3];   qb[1] = quat[2];   qb[2] = -quat[1];  qb[3] = quat[0];
    qb[4] = -quat[2];  qb[5] = quat[3];   qb[6] = quat[0];   qb[7] = quat[1];
    qb[8] = quat[1];   qb[9] = -quat[0];  qb[10] = quat[3];  qb[11] = quat[2];
    qb[12] = -quat[0]; qb[13] = -quat[1]; qb[14] = -quat[2]; qb[15] = quat[3];
    ToppleMtx4Mul(rotMtx, qa, qb);
    /* vidt.q R003 and C030: w row cleared, w column (0, 0, 0, 1) */
    rotMtx[3] = 0.0f;
    rotMtx[7] = 0.0f;
    rotMtx[11] = 0.0f;
    rotMtx[12] = self->base.base.pos[0];
    rotMtx[14] = self->base.base.pos[2];
    rotMtx[15] = 1.0f;
    t = self->toppleT;
    self->toppleRise = self->toppleRise + (1.0f - __builtin_cosf(t * t * 3.1415927f)) * 0.5f * 25.0f;
    rotMtx[13] = self->toppleRise;
    yawMtx[13] = self->base.base.pos[1];
    /* yaw rotation about y (vrot of rot.y * 2/pi), then root = rotMtx * (yaw * yawMtx) */
    angle = self->base.base.rot[1];
    c = __builtin_cosf(angle);
    s = __builtin_sinf(angle);
    yawRot[0] = c;     yawRot[1] = 0.0f;  yawRot[2] = -s;    yawRot[3] = 0.0f;
    yawRot[4] = 0.0f;  yawRot[5] = 1.0f;  yawRot[6] = 0.0f;  yawRot[7] = 0.0f;
    yawRot[8] = s;     yawRot[9] = 0.0f;  yawRot[10] = c;    yawRot[11] = 0.0f;
    yawRot[12] = 0.0f; yawRot[13] = 0.0f; yawRot[14] = 0.0f; yawRot[15] = 1.0f;
    ToppleMtx4Mul(yawMtx, yawRot, yawMtx);
    ToppleMtx4Mul(world, rotMtx, yawMtx);
    mtx = self->base.base.data->rootMatrix;
    for (i = 0; i < 16; i++) {
      mtx[i] = world[i];
    }
    break;
  case 2:
    ActorStageObjPropPlaySound(self, 0x2000e6);
    self->toppleTimer = 30;
    if (ActorStageObjIsBillboard(&self->base) != 0) {
      for (i = 0; i < 8; i++) {
        mtx = self->base.base.data->rootMatrix;
        bounds = ActorStageObjGetBounds(&self->base);
        h = bounds[5] * 2.0f;
        y = h * (PlatformRandFloat12() - 1.0f);
        bounds = ActorStageObjGetBounds(&self->base);
        local.x = 0.0f;
        local.y = y - bounds[5];
        local.z = 0.0f;
        local.w = 0.0f;
        MathMtx4TransformPoint((const ScePspFMatrix4 *)mtx, &out, &local);
        spawnPos[0] = out.x;
        spawnPos[1] = out.y;
        spawnPos[2] = out.z;
        spawnPos[3] = out.w;
        dir[0] = self->base.base.pos[0] - spawnPos[0];
        dir[1] = 0.0f;
        dir[2] = self->base.base.pos[2] - spawnPos[2];
        dir[3] = self->base.base.pos[3];
        ToppleNormalize3(dir);
        GfxEffectSpawnDirected(g_btlUnitEffectMgr, 0xd, spawnPos, dir);
      }
      self->toppleTimer = 60;
    }
    self->step = self->step + 1;
    break;
  case 3:
    if (self->toppleTimer == 0) {
      self->step = self->step + 1;
    } else {
      self->toppleTimer = self->toppleTimer - 1;
    }
    break;
  case 4:
    if (self->toppleTimer == 0x20) {
      GfxModelForEachMaterial(&self->base.base, (void *)ActorStageObjPropToppleMaterialSetTranslucent,
                              NULL);
      self->base.fade = 1.0f;
      self->step = self->step + 1;
    } else {
      if ((self->toppleTimer & 2) != 0) {
        fade = 1.0f;
      } else {
        fade = 0.0f;
      }
      self->base.fade = fade;
      self->toppleTimer = self->toppleTimer + 1;
    }
    break;
  case 5:
    fade = self->base.fade - 0.05f;
    self->base.fade = fade;
    if (fade <= 0.0f) {
      self->base.fade = 0.0f;
      self->step = self->step + 1;
    }
    break;
  case 6:
    brk = &((const VtblEntry *)self->base.base.base.vtable)[11];
    ((void (*)(void *))brk->fn)((u8 *)self + brk->delta);
    self->step = self->step + 1;
    break;
  }
}
