// bdc 0x08824a78 GfxEffectMgrDrawModels
#include "bdc.h"

/* d = a * b in the engine's column-major layout (16 floats, column j at [j * 4]): column j of d is
   the sum over k of b[j][k] * column k of a. Both inputs are read before d is written, so d may
   alias a or b. The asm's `vmmul.q M000, M100, M200` with M100 = a, M200 = b, and its
   `vmmul.q E_d, E_s, E_t` with a = the E_t operand and b = the E_s operand. */
static void GfxEffectMatMul(float *d, const float *a, const float *b)
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

/* Roll about Z by `angle` (radians, scaled to quarter turns by the bank's 2/pi, S703):
   columns (c, s, 0, 0), (-s, c, 0, 0), (0, 0, 1, 0), (0, 0, 0, 1). */
static void GfxEffectRollMatrix(float *d, float angle)
{
  float q = angle * 0.636619747f;
  float c = VfCosQuarter(q);
  float s = VfSinQuarter(q);
  int i;

  for (i = 0; i < 16; i++) {
    d[i] = 0.0f;
  }
  d[0] = c;
  d[1] = s;
  d[4] = -s;
  d[5] = c;
  d[10] = 1.0f;
  d[15] = 1.0f;
}

/* d = roll(angle) * diag(size.x, size.y, size.z, 1) (`vmidt.q M300` + size on the diagonal,
   `vmmul.q E200, E300, E000`). */
static void GfxEffectScaledRoll(float *d, const float *size, float angle)
{
  float diag[16];
  float rot[16];
  int i;

  for (i = 0; i < 16; i++) {
    diag[i] = 0.0f;
  }
  diag[0] = size[0];
  diag[5] = size[1];
  diag[10] = size[2];
  diag[15] = 1.0f;
  GfxEffectRollMatrix(rot, angle);
  GfxEffectMatMul(d, rot, diag);
}

/* 1 / |v.xyz|, or 0 for a zero-length vector (`vrsq.s` + `vcmovt.s` of the bank's 0, S713). */
static float GfxEffectInvLength3(const float *v)
{
  float lenSq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];

  if (lenSq == 0.0f) {
    return 0.0f;
  }
  return VfRsq(lenSq);
}

/* Draws the model-type effects of an effect manager (`GfxEffectMgrCtor`): collects the effects with
   a model (`model`) sorted back-to-front into `g_gfxEffectModelDepthList`
   (`GfxEffectMgrBuildDepthList`, `GfxCombSortByDepth`); returns at once when there is none.
   Otherwise opens a chunk on `packet`, writes the light state for `camera`
   (`GfxDlWriteLightState`) and the light-0 vector `g_gfxEffectModelLightDir` (initialised once,
   `g_gfxEffectModelLightInit`; rotated about Y by 0.08 and renormalised, w cleared, after it is
   written), then GE command 0x53000003. For each effect it builds the model's root matrix by `kind`:
   0 camera billboard x roll(`worldDir[2]`) x diag(size), 1 the effect matrix, 2 the effect matrix
   columns x/y/z scaled by `size`, 3 the rotation of the quaternion at `+0xa0`, 4 a look-along
   basis (forward `toCamera` / up `worldDir` when flags & 0x10, else forward `dir` / up +Y, or +Z
   when `dir` is nearly vertical, then x roll) with columns x/y/z scaled by `size`, 10 effect matrix
   x roll x diag(size), other kinds keep the old rotation; puts `pos` in the w column, applies
   `blendMode & 0xffff` to every material (`GfxModelForEachMaterial` with
   `GfxEffectMaterialSetBlend`), copies `color` to the model's `ambient` and calls the model's draw
   method (vtable `+0x40/+0x44`) on the chunk cursor. Finally closes the chunk (`GfxPacketEndChunk`). */

void GfxEffectMgrDrawModels(GfxEffectMgr *mgr, void *packet, void *camera)

{
  GfxCamera *cam = (GfxCamera *)camera;
  float rot[16];
  union {
    float f;
    u32 u;
  } bits;
  GfxEffectDepthEntry *entry;
  u32 *cursor;
  u32 blendMode;
  u32 count;
  s32 i;
  int k;

  if (g_gfxEffectModelLightInit == 0) {
    g_gfxEffectModelLightInit = 1;
    g_gfxEffectModelLightDir[0] = -1.29999995f;
    g_gfxEffectModelLightDir[1] = 1.52499998f;
    g_gfxEffectModelLightDir[2] = 3.77999997f;
    g_gfxEffectModelLightDir[3] = 0.0f;
  }
  count = GfxEffectMgrBuildDepthList(mgr->base.head, g_gfxEffectModelDepthList, camera);
  if (count == 0) {
    return;
  }
  cursor = GfxPacketBeginChunk((RenderPacket *)packet);
  cursor = GfxDlWriteLightState(cursor, cam, 1);
  bits.f = g_gfxEffectModelLightDir[0];
  cursor[0] = bits.u >> 8 | 0x63000000;
  bits.f = g_gfxEffectModelLightDir[1];
  cursor[1] = bits.u >> 8 | 0x64000000;
  bits.f = g_gfxEffectModelLightDir[2];
  cursor[2] = bits.u >> 8 | 0x65000000;
  cursor = cursor + 3;
  {
    /* rotate the light vector about Y by 0.08 (0x3da3d70a): x' = (x, y, z) . (c, 0, -s),
       z' = (x, y, z) . (s, 0, c); y and w stay */
    float *light = g_gfxEffectModelLightDir;
    float q = 0.08f * 0.636619747f;
    float c = VfCosQuarter(q);
    float s = VfSinQuarter(q);
    float x = light[0];
    float y = light[1];
    float z = light[2];
    float inv;

    light[0] = x * c + y * 0.0f + z * -s;
    light[2] = x * s + y * 0.0f + z * c;
    /* then normalise x/y/z in place (clamped to [-1, 1]); w becomes the bank's 0 (lane 3 of C710) */
    inv = GfxEffectInvLength3(light);
    light[0] = VfSat1(light[0] * inv);
    light[1] = VfSat1(light[1] * inv);
    light[2] = VfSat1(light[2] * inv);
    light[3] = 0.0f;
  }
  *cursor = 0x53000003;
  cursor = cursor + 1;
  blendMode = 0xffffffff;
  GfxCombSortByDepth(g_gfxEffectModelDepthList, count);
  entry = g_gfxEffectModelDepthList;
  for (i = 0; i < (s32)count; i++) {
    GfxEffect *effect = entry->effect;
    GfxModel *model = effect->model;
    float *m = model->data->rootMatrix;
    const GfxModelVtable *vt;

    switch (effect->kind) {
    case 0:
      /* m = billboard x (roll x diag(size)) */
      GfxEffectScaledRoll(m, effect->size, effect->worldDir[2]);
      GfxEffectMatMul(m, (const float *)&cam->billboard, m);
      break;
    case 1:
      /* the effect matrix (its w column is overwritten by pos below) */
      for (k = 0; k < 16; k++) {
        m[k] = effect->matrix[k];
      }
      break;
    case 2:
      /* the effect matrix columns x/y/z scaled by size */
      for (k = 0; k < 4; k++) {
        m[0 + k] = effect->matrix[0 + k] * effect->size[0];
        m[4 + k] = effect->matrix[4 + k] * effect->size[1];
        m[8 + k] = effect->matrix[8 + k] * effect->size[2];
      }
      break;
    case 3: {
      /* rotation matrix of the quaternion q = (X, Y, Z, W) at +0xa0: A = columns
         (W, Z, -Y, -X), (-Z, W, X, -Y), (Y, -X, W, -Z), q; B = columns (W, Z, -Y, X),
         (-Z, W, X, Y), (Y, -X, W, Z), (-X, -Y, -Z, W); m = A x B with lane 3 of the x/y/z columns
         0 and the w column (0, 0, 0, 1) */
      const float *quat = effect->quat;
      float qx = quat[0];
      float qy = quat[1];
      float qz = quat[2];
      float qw = quat[3];
      float a[16];
      float b[16];

      a[0] = qw;
      a[1] = qz;
      a[2] = -qy;
      a[3] = -qx;
      a[4] = -qz;
      a[5] = qw;
      a[6] = qx;
      a[7] = -qy;
      a[8] = qy;
      a[9] = -qx;
      a[10] = qw;
      a[11] = -qz;
      a[12] = qx;
      a[13] = qy;
      a[14] = qz;
      a[15] = qw;
      b[0] = qw;
      b[1] = qz;
      b[2] = -qy;
      b[3] = qx;
      b[4] = -qz;
      b[5] = qw;
      b[6] = qx;
      b[7] = qy;
      b[8] = qy;
      b[9] = -qx;
      b[10] = qw;
      b[11] = qz;
      b[12] = -qx;
      b[13] = -qy;
      b[14] = -qz;
      b[15] = qw;
      GfxEffectMatMul(m, a, b);
      m[3] = 0.0f;
      m[7] = 0.0f;
      m[11] = 0.0f;
      m[12] = 0.0f;
      m[13] = 0.0f;
      m[14] = 0.0f;
      m[15] = 1.0f;
      break;
    }
    case 4: {
      const float *fwd;
      const float *up;
      float f[3];
      float x[3];
      float inv;
      u8 followed = (effect->flags & 0x10) != 0;

      if (followed) {
        fwd = effect->toCamera;
        up = effect->worldDir;
      } else {
        float xz = effect->dir[0] * effect->dir[0];
        float zz = effect->dir[2] * effect->dir[2];
        u8 vertical;

        xz = xz + zz;
        if (xz < 1e-05f) {
          float yy = effect->dir[1] * effect->dir[1];
          vertical = !(yy <= 0.0001f);
        } else {
          vertical = 0;
        }
        fwd = effect->dir;
        if (vertical != 0) {
          up = (const float *)&g_gfxSpriteSubList; /* +Z */
        } else {
          up = (const float *)&g_vecUp;
        }
      }
      /* orthonormal basis: z = normalised forward, x = up x z normalised, y = z x x
         (each normalisation clamped to [-1, 1]); lane 3 of x/y/z 0, w column (0, 0, 0, 1) */
      inv = GfxEffectInvLength3(fwd);
      f[0] = VfSat1(fwd[0] * inv);
      f[1] = VfSat1(fwd[1] * inv);
      f[2] = VfSat1(fwd[2] * inv);
      x[0] = up[1] * f[2] - up[2] * f[1];
      x[1] = up[2] * f[0] - up[0] * f[2];
      x[2] = up[0] * f[1] - up[1] * f[0];
      inv = GfxEffectInvLength3(x);
      x[0] = VfSat1(x[0] * inv);
      x[1] = VfSat1(x[1] * inv);
      x[2] = VfSat1(x[2] * inv);
      m[0] = x[0];
      m[1] = x[1];
      m[2] = x[2];
      m[3] = 0.0f;
      m[4] = f[1] * x[2] - f[2] * x[1];
      m[5] = f[2] * x[0] - f[0] * x[2];
      m[6] = f[0] * x[1] - f[1] * x[0];
      m[7] = 0.0f;
      m[8] = f[0];
      m[9] = f[1];
      m[10] = f[2];
      m[11] = 0.0f;
      m[12] = 0.0f;
      m[13] = 0.0f;
      m[14] = 0.0f;
      m[15] = 1.0f;
      if (!followed) {
        /* m = m x roll(worldDir[2]) */
        GfxEffectRollMatrix(rot, effect->worldDir[2]);
        GfxEffectMatMul(m, m, rot);
      }
      /* columns x/y/z scaled by size */
      for (k = 0; k < 4; k++) {
        m[0 + k] = m[0 + k] * effect->size[0];
        m[4 + k] = m[4 + k] * effect->size[1];
        m[8 + k] = m[8 + k] * effect->size[2];
      }
      break;
    }
    case 10:
      /* m = effect matrix x (roll x diag(size)) */
      GfxEffectScaledRoll(m, effect->size, effect->worldDir[2]);
      GfxEffectMatMul(m, effect->matrix, m);
      break;
    default:
      break;
    }
    /* w column (translation) = pos */
    m[12] = effect->pos[0];
    m[13] = effect->pos[1];
    m[14] = effect->pos[2];
    m[15] = effect->pos[3];
    blendMode = effect->blendMode & 0xffff;
    GfxModelForEachMaterial(model, (void *)GfxEffectMaterialSetBlend, &blendMode);
    model->ambient[0] = effect->color[0];
    model->ambient[1] = effect->color[1];
    model->ambient[2] = effect->color[2];
    model->ambient[3] = effect->color[3];
    vt = (const GfxModelVtable *)model->base.vtable;
    vt->draw((u8 *)model + vt->drawAdjust, &cursor);
    entry = entry + 1;
  }
  GfxPacketEndChunk((RenderPacket *)packet, cursor);
  return;
}
