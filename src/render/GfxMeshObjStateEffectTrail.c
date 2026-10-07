// bdc 0x088273b4 GfxMeshObjStateEffectTrail
#include "bdc.h"

/* State 2 state handler of the mesh object (`GfxMeshObjCtor`) (table `0x08ab9ebc`, run by
   `GfxMeshObjRunState`): builds and animates the ribbon/ring mesh of its owner
   `GfxEffect` (`owner`) from the mesh object's `GfxEffectChain` points (`effectChain`) and the
   per-point float buffer `vertexBuffer` (`GfxEffectAllocTrailBuffer`). Sub-state `step`:
   0xdeae deletes itself through the virtual destructor (nothing when `self` is NULL);
   0xdead hides it (`visible` 0) and advances `step`; any step other than 0/1 returns at once.
   Step 0 (setup, then falls through to the per-frame build) takes the effect's texture and, by the
   effect `kind`:
   5   ribbon spline: `patchCountU` 4, `patchCountV` = chain count - 1, `trailLength` 2 × that,
       index table `g_gfxTrailSplineIndices`, `splineEdgeU` 3, VTYPE 0x1200099c;
   0xb/0xc grid: `trailLength` = U × V, an allocated identity byte index table (`flags` bit 1),
       VTYPE 0x1200019c while filling, then 0x1200099c, `splineEdgeU` 3;
   other ring: an allocated byte index table of V rows of U + 3 indices wrapping round the U ring
       vertices (`flags` bit 1), `trailLength` = U × V, `axis[0]` = 2pi / U (angle step),
       `axis[1]` = 3 sqrt(1 / U) (ring radius), `patchCountU` += 3, VTYPE 0x1200099c.
   Each allocates (low heap, `MemAlloc` under `MemLock`) a double-buffered
   `GfxGeColorVertexF` vertex buffer (`buffer`, 2 × `trailLength` vertices) and fills the colours of
   both halves with white whose alpha comes from the per-point buffer (each channel clamped to
   [0, 1], × 255 and truncated to a byte). `scaleC` = 1 / (U - 3) and `radius` = 1 / (V - 3) (ring
   kind only when both counts are >= 4); then `emissive` 1, `pos` = 0, `visible` 1, `step` + 1.
   Every frame (step 1 and after setup): `blendMode` = effect blend mode & 0xffff, `clut` = effect
   texture slot, `texture`, `indices` = this frame's half of `buffer` (`g_gfxFrameIndex`), then
   kind 5: per segment the normalised direction to the next point, re-oriented by `mode`
   (0 (x,-z,y), 1 (-z,y,x), 2 (-y,x,z), 3 world up `g_vecUp`, 4 cross with the direction from the
   camera eye, >= 5 unchanged), scaled by the point's width (first third of `vertexBuffer`) gives the
   two vertices point ± offset; basis = effect matrix with rows scaled by the effect size;
   0xb/0xc: vertices = the chain points; 0xb basis = followed matrix (`attachMatrix`, else the effect
   matrix) scaled by size; 0xc basis = diag(size), turned to face the normalised effect `dir`
   (`MathMat4LookDir` with world up, or the Y/Z swap `g_gfxSwapYZMatrix` when `dir` is nearly
   vertical), then a Y rotation by `worldDir[2]` and the followed/effect matrix;
   other: each of the U - 3 ring angles (`axis[0]` × i - pi, advanced per point by the third third of
   `vertexBuffer`) rotates the point's x/z (Y rotation, `axis[1]` scale) about a centre that is the
   chain's `velocities` entry (or `g_gfxVecZero` once `ready` is set); kind 6 basis as 0xb; kind 7
   basis = (Z rotation by `worldDir[2]`) × diag(size), combined with the camera `billboard` and then
   with (followed/effect matrix × Y/Z swap); other kinds as 0xc but starting from the Y/Z swap scaled
   by size and with the look-at built inline. All kinds then copy the effect position to `pos`, set
   `texScaleU` = scaleA × scaleC, `texScaleV` = scaleB × radius and copy the effect colour to `color`.
   `vertices`/`indices` hold the GE index and vertex addresses respectively (see
   `GfxMeshObjDrawList`). Matrices are 16 floats, four 4-float fields (column-major). */

/* dst = src, 4 floats */
static void TrailCopy4(float *dst, const float *src)
{
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

/* dst = src, a 4x4 matrix */
static void TrailCopy16(float *dst, const float *src)
{
  s32 i;

  for (i = 0; i < 16; i++) {
    dst[i] = src[i];
  }
}

/* basis fields 0..2 (4 floats each) scaled by size x/y/z */
static void TrailScaleBasis(float *basis, const float *size)
{
  s32 r;
  s32 c;

  for (r = 0; r < 3; r++) {
    for (c = 0; c < 4; c++) {
      basis[r * 4 + c] = basis[r * 4 + c] * size[r];
    }
  }
}

/* out = a * b (column-major, field j of out = sum over k of b[j][k] * field k of a);
   out may alias a or b */
static void TrailMat4Mul(float *out, const float *a, const float *b)
{
  float r[16];
  s32 i;
  s32 j;

  for (j = 0; j < 4; j++) {
    for (i = 0; i < 4; i++) {
      r[j * 4 + i] = b[j * 4 + 0] * a[0 + i] + b[j * 4 + 1] * a[4 + i] + b[j * 4 + 2] * a[8 + i]
                     + b[j * 4 + 3] * a[12 + i];
    }
  }
  TrailCopy16(out, r);
}

/* out.xyz = normalise(in.xyz), each lane clamped to [-1, 1]; a zero-length vector is scaled by 0.
   out may alias in. */
static void TrailNormalize(float *out, const float *in)
{
  float x;
  float y;
  float z;
  float s;

  x = in[0];
  y = in[1];
  z = in[2];
  s = x * x + y * y + z * z;
  if (s == 0.0f) {
    s = 0.0f;
  } else {
    s = VfRsq(s);
  }
  out[0] = VfSat1(x * s);
  out[1] = VfSat1(y * s);
  out[2] = VfSat1(z * s);
}

/* out.xyz = a x b; out may alias a or b */
static void TrailCross(float *out, const float *a, const float *b)
{
  float x;
  float y;
  float z;

  x = a[1] * b[2] - a[2] * b[1];
  y = a[2] * b[0] - a[0] * b[2];
  z = a[0] * b[1] - a[1] * b[0];
  out[0] = x;
  out[1] = y;
  out[2] = z;
}

/* RGBA float colour to an RGBA8 word: each channel clamped to [0, 1], × 255, truncated */
static u32 TrailPackColor(const float *color)
{
  u32 rgba;
  s32 i;

  rgba = 0;
  for (i = 0; i < 4; i++) {
    rgba |= (u32)VfI2uc(VfF2iz(VfSat0(color[i]) * 255.0f, 23)) << (i * 8);
  }
  return rgba;
}

/* out = Y rotation by angle: fields (c,0,-s,0), (0,1,0,0), (s,0,c,0), (0,0,0,1) */
static void TrailRotY(float *out, float angle)
{
  float c;
  float s;

  c = __builtin_cosf(angle);
  s = __builtin_sinf(angle);
  out[0] = c;
  out[1] = 0.0f;
  out[2] = -s;
  out[3] = 0.0f;
  out[4] = 0.0f;
  out[5] = 1.0f;
  out[6] = 0.0f;
  out[7] = 0.0f;
  out[8] = s;
  out[9] = 0.0f;
  out[10] = c;
  out[11] = 0.0f;
  out[12] = 0.0f;
  out[13] = 0.0f;
  out[14] = 0.0f;
  out[15] = 1.0f;
}

/* out = identity */
static void TrailIdentity(float *out)
{
  s32 i;

  for (i = 0; i < 16; i++) {
    out[i] = (i % 5 == 0) ? 1.0f : 0.0f;
  }
}

/* out = look-at orientation from dir and world up g_vecUp, rows x, y, z (MathMat4LookDir's code,
   inlined): z = normalise(dir), x = normalise(up x z), y = z x x */
static void TrailLookDirUp(float *out, const float *dir)
{
  float z[3];
  float x[3];
  float y[3];

  TrailNormalize(z, dir);
  TrailCross(x, &g_vecUp.x, z);
  TrailNormalize(x, x);
  TrailCross(y, z, x);
  out[0] = x[0];
  out[1] = x[1];
  out[2] = x[2];
  out[3] = 0.0f;
  out[4] = y[0];
  out[5] = y[1];
  out[6] = y[2];
  out[7] = 0.0f;
  out[8] = z[0];
  out[9] = z[1];
  out[10] = z[2];
  out[11] = 0.0f;
  out[12] = 0.0f;
  out[13] = 0.0f;
  out[14] = 0.0f;
  out[15] = 1.0f;
}

/* nearly vertical unit direction: x² + z² < 1e-5 and not y² <= 1e-4 */
static bool TrailIsVertical(const float *dir)
{
  if (dir[0] * dir[0] + dir[2] * dir[2] < 9.99999975e-06f) {
    if (dir[1] * dir[1] <= 9.99999975e-05f) {
      return false;
    }
    return true;
  }
  return false;
}

void GfxMeshObjStateEffectTrail(GfxMeshObj *self)
{
  GfxEffect *effect;
  s32 step;
  bool wasLow;
  u8 *idx;
  const float *alpha;
  const float *widths;
  const float *src;
  const ScePspFVector4 *p;
  const ScePspFVector4 *center;
  const VtblEntry *dtor;
  u32 rgba;
  s32 n;
  s32 side;
  s32 i;
  s32 u;
  s32 v;
  s32 w;
  s32 segs;
  s32 base;
  s32 k;
  float angle;
  float radius;
  float c;
  float s;
  float rx;
  float rz;
  float t;
  float width;
  GfxGeColorVertexF *vert;
  float color[4];
  float dir[4];
  float toEye[4];
  float side3[4];
  float look[16];
  float tmpM[16];
  float diag[16];
  void *buf;

  step = self->step;
  effect = self->owner;
  if (step == 0xdeae) {
    if (self != NULL) {
      dtor = &((const VtblEntry *)self->base.vtable)[1];
      ((void (*)(void *, s32))dtor->fn)((u8 *)self + dtor->delta, 3);
    }
    return;
  }
  if (step == 0xdead) {
    self->visible = 0;
    self->step = self->step + 1;
    return;
  }
  if (step != 1) {
    if (step != 0) {
      return;
    }

    /* ---- step 0: setup ---- */
    self->texture = effect->texture;
    if (effect->kind == 5) {
      self->splineEdgeU = 3;
      self->vertices = g_gfxTrailSplineIndices;
      self->patchCountU = 4;
      n = self->effectChain.count - 1;
      self->patchCountV = n;
      self->u158.trailLength = n * 2;
      self->vertexType = 0x1200099c;
      MemLock();
      wasLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      buf = MemAlloc(n * 2 * 2 * (s32)sizeof(GfxGeColorVertexF), NULL, 0);
      MemSetAllocFromLow(wasLow);
      MemUnlock();
      self->buffer = buf;
      TrailCopy4(color, &g_colorWhite.x);
      alpha = (const float *)self->vertexBuffer + self->patchCountV + 1;
      for (side = 0; side < 2; side++) {
        for (i = 0, k = 0; i < self->patchCountV; i++, k += 2) {
          color[3] = alpha[i];
          rgba = TrailPackColor(color);
          ((GfxGeColorVertexF *)self->buffer)[self->u158.trailLength * side + k].colour = rgba;
          ((GfxGeColorVertexF *)self->buffer)[self->u158.trailLength * side + k + 1].colour = rgba;
        }
      }
      self->scaleC = 1.0f / (float)(self->patchCountU - 3);
      self->radius = 1.0f / (float)(self->patchCountV - 3);
    } else if (effect->kind == 0xb || effect->kind == 0xc) {
      n = self->patchCountU * self->patchCountV;
      self->u158.trailLength = n;
      self->vertexType = 0x1200019c;
      self->splineEdgeU = 3;
      self->vertices = NULL;
      MemLock();
      wasLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      idx = MemAlloc(n, NULL, 0);
      MemSetAllocFromLow(wasLow);
      MemUnlock();
      self->flags |= 2;
      self->vertices = idx;
      for (i = 0; i < self->u158.trailLength; i++) {
        idx[i] = (u8)i;
      }
      n = self->u158.trailLength;
      MemLock();
      wasLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      buf = MemAlloc(n * 2 * (s32)sizeof(GfxGeColorVertexF), NULL, 0);
      MemSetAllocFromLow(wasLow);
      MemUnlock();
      self->buffer = buf;
      TrailCopy4(color, &g_colorWhite.x);
      for (side = 0; side < 2; side++) {
        for (v = 0; v < self->patchCountV; v++) {
          alpha = (const float *)self->vertexBuffer + self->u158.trailLength
                  + v * self->patchCountU;
          for (u = 0; u < self->patchCountU; u++) {
            color[3] = alpha[u];
            rgba = TrailPackColor(color);
            ((GfxGeColorVertexF *)self->buffer)[self->u158.trailLength * side
                                                + v * self->patchCountU + u].colour = rgba;
          }
        }
      }
      self->vertexType = 0x1200099c;
      self->scaleC = 1.0f / (float)(self->patchCountU - 3);
      self->radius = 1.0f / (float)(self->patchCountV - 3);
    } else {
      MemLock();
      wasLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      idx = MemAlloc((self->patchCountU + 3) * self->patchCountV, NULL, 0);
      MemSetAllocFromLow(wasLow);
      MemUnlock();
      self->flags |= 2;
      self->vertices = idx;
      for (v = 0; v < self->patchCountV; v++) {
        for (u = 0; u < self->patchCountU + 3; u++) {
          w = u;
          if (!(u < self->patchCountU)) {
            w = u - self->patchCountU;
          }
          idx[(self->patchCountU + 3) * v + u] = (u8)(v * self->patchCountU + w);
        }
      }
      u = self->patchCountU;
      n = u * self->patchCountV;
      self->u158.trailLength = n;
      self->axis[0] = 6.28318548f / (float)u;
      self->axis[1] = __builtin_sqrtf(1.0f / (float)u) * 3.0f;
      self->patchCountU = u + 3;
      self->vertexType = 0x1200099c;
      MemLock();
      wasLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      buf = MemAlloc(n * 2 * (s32)sizeof(GfxGeColorVertexF), NULL, 0);
      MemSetAllocFromLow(wasLow);
      MemUnlock();
      self->buffer = buf;
      TrailCopy4(color, &g_colorWhite.x);
      segs = self->patchCountU - 3;
      for (side = 0; side < 2; side++) {
        for (v = 0, base = 0; v < self->patchCountV; v++, base += segs) {
          color[3] = ((const float *)self->vertexBuffer)[self->patchCountV + 1 + v];
          rgba = TrailPackColor(color);
          for (i = 0; i < segs; i++) {
            ((GfxGeColorVertexF *)self->buffer)[self->u158.trailLength * side + base + i].colour =
                rgba;
          }
        }
      }
      if (self->patchCountU >= 4 && self->patchCountV >= 4) {
        self->scaleC = 1.0f / (float)(self->patchCountU - 3);
        self->radius = 1.0f / (float)(self->patchCountV - 3);
      }
    }
    self->emissive = 1;
    /* the bank's zero vector C720 */
    self->pos[0] = 0.0f;
    self->pos[1] = 0.0f;
    self->pos[2] = 0.0f;
    self->pos[3] = 0.0f;
    self->visible = 1;
    self->step = self->step + 1;
  }

  /* ---- every frame ---- */
  self->blendMode = effect->blendMode & 0xffff;
  self->clut = effect->textureSlot;
  self->texture = effect->texture;
  self->indices = (GfxGeColorVertexF *)self->buffer + self->u158.trailLength * g_gfxFrameIndex;

  if (effect->kind == 5) {
    widths = self->vertexBuffer;
    for (i = 0; i < self->patchCountV; i++) {
      p = &self->effectChain.points[i];
      dir[0] = self->effectChain.points[i + 1].x - p->x;
      dir[1] = self->effectChain.points[i + 1].y - p->y;
      dir[2] = self->effectChain.points[i + 1].z - p->z;
      TrailNormalize(dir, dir);
      switch (self->mode) {
      case 0:
        t = dir[1];
        dir[1] = -dir[2];
        dir[2] = t;
        break;
      case 1:
        t = dir[0];
        dir[0] = -dir[2];
        dir[2] = t;
        break;
      case 2:
        t = dir[0];
        dir[0] = -dir[1];
        dir[1] = t;
        break;
      case 3:
        TrailCopy4(dir, &g_vecUp.x);
        break;
      case 4:
        p = &self->effectChain.points[i];
        toEye[0] = p->x - g_gfxActiveCamera->eye[0];
        toEye[1] = p->y - g_gfxActiveCamera->eye[1];
        toEye[2] = p->z - g_gfxActiveCamera->eye[2];
        TrailNormalize(toEye, toEye);
        TrailCross(dir, dir, toEye);
        break;
      default:
        break;
      }
      width = widths[i];
      dir[0] = dir[0] * width;
      dir[1] = dir[1] * width;
      dir[2] = dir[2] * width;
      p = &self->effectChain.points[i];
      side3[0] = p->x + dir[0];
      side3[1] = p->y + dir[1];
      side3[2] = p->z + dir[2];
      ((GfxGeColorVertexF *)self->indices)[i * 2].x = side3[0];
      ((GfxGeColorVertexF *)self->indices)[i * 2].y = side3[1];
      ((GfxGeColorVertexF *)self->indices)[i * 2].z = side3[2];
      p = &self->effectChain.points[i];
      side3[0] = p->x - dir[0];
      side3[1] = p->y - dir[1];
      side3[2] = p->z - dir[2];
      ((GfxGeColorVertexF *)self->indices)[i * 2 + 1].x = side3[0];
      ((GfxGeColorVertexF *)self->indices)[i * 2 + 1].y = side3[1];
      ((GfxGeColorVertexF *)self->indices)[i * 2 + 1].z = side3[2];
    }
    TrailCopy16(self->basis, effect->matrix);
    TrailScaleBasis(self->basis, effect->size);
    TrailCopy4(self->pos, effect->pos);
  } else if (effect->kind == 0xb || effect->kind == 0xc) {
    for (u = 0; u < self->patchCountU; u++) {
      for (v = 0; v < self->patchCountV; v++) {
        k = v * self->patchCountU + u;
        vert = self->indices;
        vert[k].x = self->effectChain.points[k].x;
        vert = self->indices;
        vert[k].y = self->effectChain.points[k].y;
        vert = self->indices;
        vert[k].z = self->effectChain.points[k].z;
      }
    }
    if (effect->kind == 0xb) {
      if (effect->attachMatrix != NULL) {
        src = effect->attachMatrix;
      } else {
        src = effect->matrix;
      }
      TrailCopy16(self->basis, src);
      TrailScaleBasis(self->basis, effect->size);
      TrailCopy4(self->pos, effect->pos);
    } else {
      TrailIdentity(self->basis);
      self->basis[0] = effect->size[0];
      self->basis[5] = effect->size[1];
      self->basis[10] = effect->size[2];
      TrailNormalize(dir, effect->dir);
      if (TrailIsVertical(dir)) {
        TrailMat4Mul(self->basis, self->basis, (const float *)&g_gfxSwapYZMatrix);
      } else {
        TrailMat4Mul(self->basis, MathMat4LookDir(look, dir, &g_vecUp.x), self->basis);
      }
      TrailRotY(look, effect->worldDir[2]);
      if (effect->attachMatrix != NULL) {
        TrailMat4Mul(tmpM, look, effect->attachMatrix);
      } else {
        TrailMat4Mul(tmpM, look, effect->matrix);
      }
      TrailMat4Mul(self->basis, self->basis, tmpM);
      TrailCopy4(self->pos, effect->pos);
    }
  } else {
    /* ring: U - 3 angles round each chain point */
    segs = self->patchCountU - 3;
    widths = (const float *)self->vertexBuffer + 2 * self->patchCountV + 2;
    center = &g_gfxVecZero;
    for (i = 0; i < segs; i++) {
      angle = self->axis[0] * (float)i - 3.14159274f;
      if (!self->effectChain.ready) {
        center = self->effectChain.velocities;
      }
      for (v = 0; v < self->patchCountV; v++) {
        vert = (GfxGeColorVertexF *)self->indices + (i + v * segs);
        radius = self->axis[1];
        p = &self->effectChain.points[v];
        /* x/z rotated by the Y rotation of `angle` (fields (c,0,-s), (s,0,c)), scaled by the
           radius, y kept, plus the centre */
        c = __builtin_cosf(angle);
        s = __builtin_sinf(angle);
        rx = p->x * c + p->y * 0.0f + p->z * -s;
        rz = p->x * s + p->y * 0.0f + p->z * c;
        vert->x = rx * radius + center->x;
        vert->y = p->y + center->y;
        vert->z = rz * radius + center->z;
        angle = angle + widths[v];
        if (!self->effectChain.ready) {
          center = center + 1;
        }
      }
    }
    if (effect->kind == 6) {
      if (effect->attachMatrix != NULL) {
        src = effect->attachMatrix;
      } else {
        src = effect->matrix;
      }
      TrailCopy16(self->basis, src);
      TrailScaleBasis(self->basis, effect->size);
      TrailCopy4(self->pos, effect->pos);
    } else if (effect->kind == 7) {
      /* basis = (Z rotation by worldDir[2]) × diag(size) */
      TrailIdentity(diag);
      diag[0] = effect->size[0];
      diag[5] = effect->size[1];
      diag[10] = effect->size[2];
      c = __builtin_cosf(effect->worldDir[2]);
      s = __builtin_sinf(effect->worldDir[2]);
      TrailIdentity(look);
      look[0] = c;
      look[1] = s;
      look[4] = -s;
      look[5] = c;
      TrailMat4Mul(self->basis, look, diag);
      TrailMat4Mul(self->basis, (const float *)&g_gfxActiveCamera->billboard, self->basis);
      if (effect->attachMatrix != NULL) {
        TrailMat4Mul(tmpM, effect->attachMatrix, (const float *)&g_gfxSwapYZMatrix);
      } else {
        TrailMat4Mul(tmpM, effect->matrix, (const float *)&g_gfxSwapYZMatrix);
      }
      TrailMat4Mul(self->basis, self->basis, tmpM);
      TrailCopy4(self->pos, effect->pos);
    } else {
      /* identity with fields 1 and 2 swapped (the Y/Z swap) */
      TrailIdentity(self->basis);
      self->basis[5] = 0.0f;
      self->basis[6] = 1.0f;
      self->basis[9] = 1.0f;
      self->basis[10] = 0.0f;
      TrailScaleBasis(self->basis, effect->size);
      TrailNormalize(dir, effect->dir);
      if (TrailIsVertical(dir)) {
        TrailMat4Mul(self->basis, self->basis, (const float *)&g_gfxSwapYZMatrix);
      } else {
        TrailLookDirUp(look, dir);
        TrailMat4Mul(self->basis, look, self->basis);
      }
      TrailRotY(look, effect->worldDir[2]);
      if (effect->attachMatrix != NULL) {
        TrailMat4Mul(tmpM, look, effect->attachMatrix);
      } else {
        TrailMat4Mul(tmpM, look, effect->matrix);
      }
      TrailMat4Mul(self->basis, self->basis, tmpM);
      TrailCopy4(self->pos, effect->pos);
    }
  }
  self->texScaleU = self->scaleA * self->scaleC;
  self->texScaleV = self->scaleB * self->radius;
  TrailCopy4(self->color, effect->color);
}
