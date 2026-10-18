// bdc 0x08a16d8c GmoNodeApplyAttr
#include "bdc.h"

/* Applies an animated attribute value to a 0xc0-byte node record, blended by `weight`; NULL `self`
   or `value` is a no-op, as is an unknown `type`.
   - Vector types 0x48..0x4d and 0xe1 pick a `g_gmoNodeAttrSlots` entry (index
     `min(type - 0x48, 6)`): the 16-byte vector at the entry's offset (0x48 `translate`, 0x49..0x4b
     `rotate`, 0x4c/0x4d/0xe1 `scale`) is blended toward the four floats at `value`. 0x49/0x4a
     first convert the Euler angles in `value[0..2]` (radians) to a quaternion from the half-angle
     cos/sin products (the two types differ in the signs of the cross terms), then 0x49..0x4b slerp
     the stored quaternion (shortest arc, lerp fallback when |dot| >= 0.998046875, same code as
     `GmoQuatSlerpInto`); the other types lerp `p + (q - p) * weight`. The result is stored back
     and `flags42` gets the entry's clear/set bits.
   - 0x42: `visible = (int)(value[0] + 0.5f)` (truncating).
   - 0x43: resizes the morph weights to `count` with `GmoNodeSetMorphWeights` (default list
     `g_gmoMorphDefaultWeights`) when the count differs, returning if it still differs, then
     lerps `morphWeights` toward `value`.
   - 0x47: creates the explicit `matrix` from `g_gmoIdentityMatrix` (`GmoNodeSetMatrix`) if
     absent (returning if that fails), sets flag 0x10 in `flags42` and lerps it toward `value`.
   - 0x4f: sets flag 0x40000 in `flags` and lerps `morphPos` toward `value`.
   The scalar lerps are tail calls to `GmoLerpFloats` over `count` floats. */

void GmoNodeApplyAttr(float weight, GmoNode *self, s32 type, s32 unused, const float *value, u32 count)
{
  const u8 *slot;
  float *vec;
  float *out;
  s32 idx;
  ScePspFVector4 p;
  ScePspFVector4 q;
  ScePspFVector4 r;
  ScePspFVector4 diff;
  ScePspFVector4 sum;
  float cx, cy, cz, sx, sy, sz;
  float cyz, czx, cxy, syz, szx, sxy, cxyz, sxyz;
  float d;
  float sq;
  float th;
  float wp;
  float wq;
  float inv;

  if (self == NULL) {
    return;
  }
  if (value == NULL) {
    return;
  }

  if ((u32)(type - 0x48) >= 6 && type != 0xe1) {
    if (type == 0x43) {
      if (count != self->morphCount) {
        GmoNodeSetMorphWeights(self, g_gmoMorphDefaultWeights, count);
        if (count != self->morphCount) {
          return;
        }
      }
      out = self->morphWeights;
    } else if (type < 0x44) {
      if (type != 0x42) {
        return;
      }
      self->visible = (int)(value[0] + 0.5f);
      return;
    } else if (type == 0x47) {
      out = self->matrix;
      if (out == NULL) {
        GmoNodeSetMatrix(self, (const float *)&g_gmoIdentityMatrix);
        out = self->matrix;
        if (out == NULL) {
          return;
        }
      }
      self->flags42 |= 0x10;
    } else if (type == 0x4f) {
      self->flags |= 0x40000;
      out = &self->morphPos;
    } else {
      return;
    }
    GmoLerpFloats(weight, out, out, value, count);
    return;
  }

  idx = type - 0x48;
  if (idx > 6) {
    idx = 6;
  }
  slot = g_gmoNodeAttrSlots[idx];
  /* slot[0] is the vector's offset in the PSP's 0xc0-byte node (0x50 translate, 0x60 rotate,
     0x70 scale); the named field keeps it on a host layout (PC port) */
  if (slot[0] == 0x50) {
    vec = self->translate;
  } else if (slot[0] == 0x60) {
    vec = self->rotate;
  } else {
    vec = self->scale;
  }

  q.x = value[0];
  q.y = value[1];
  q.z = value[2];
  q.w = value[3];

  if ((u32)(type - 0x49) < 2) {
    /* Euler angles (radians) -> quaternion: angle * (1/pi) quarter turns is the half angle */
    q.x = VfMul(q.x, 0.318309873f);
    q.y = VfMul(q.y, 0.318309873f);
    q.z = VfMul(q.z, 0.318309873f);
    cx = VfCosQuarter(q.x);
    sx = VfSinQuarter(q.x);
    cy = VfCosQuarter(q.y);
    sy = VfSinQuarter(q.y);
    cz = VfCosQuarter(q.z);
    sz = VfSinQuarter(q.z);
    cxy = VfMul(cx, cy);
    sxy = VfMul(sx, sy);
    cyz = VfMul(cy, cz);
    czx = VfMul(cz, cx);
    syz = VfMul(sy, sz);
    szx = VfMul(sz, sx);
    cxyz = VfMul(cxy, cz);
    sxyz = VfMul(sxy, sz);
    if (type == 0x49) {
      q.x = VfSub(VfMul(cyz, sx), VfMul(syz, cx));
      q.y = VfAdd(VfMul(czx, sy), VfMul(szx, cy));
      q.z = VfSub(VfMul(cxy, sz), VfMul(sxy, cz));
      q.w = VfAdd(cxyz, sxyz);
    } else {
      q.x = VfAdd(VfMul(cyz, sx), VfMul(syz, cx));
      q.y = VfSub(VfMul(czx, sy), VfMul(szx, cy));
      q.z = VfSub(VfMul(cxy, sz), VfMul(sxy, cz));
      q.w = VfAdd(cxyz, sxyz);
    }
  }

  p.x = vec[0];
  p.y = vec[1];
  p.z = vec[2];
  p.w = vec[3];

  if ((u32)(type - 0x49) < 3) {
    /* slerp from p toward q by weight */
    d = VfSat1(VfDot4(p.x, p.y, p.z, p.w, q.x, q.y, q.z, q.w));
    diff.x = VfMul(VfSub(q.x, p.x), weight);
    diff.y = VfMul(VfSub(q.y, p.y), weight);
    diff.z = VfMul(VfSub(q.z, p.z), weight);
    diff.w = VfMul(VfSub(q.w, p.w), weight);
    sum.x = VfMul(VfAdd(q.x, p.x), weight);
    sum.y = VfMul(VfAdd(q.y, p.y), weight);
    sum.z = VfMul(VfAdd(q.z, p.z), weight);
    sum.w = VfMul(VfAdd(q.w, p.w), weight);
    if (VfDaz(d) < 0.0f) {
      q.x = -q.x;
      q.y = -q.y;
      q.z = -q.z;
      q.w = -q.w;
    }
    if (!(VfDaz(d) < 0.998046875f)) {
      r.x = VfAdd(p.x, diff.x);
      r.y = VfAdd(p.y, diff.y);
      r.z = VfAdd(p.z, diff.z);
      r.w = VfAdd(p.w, diff.w);
    } else if (VfDaz(d) < -0.998046875f) {
      r.x = VfSub(p.x, sum.x);
      r.y = VfSub(p.y, sum.y);
      r.z = VfSub(p.z, sum.z);
      r.w = VfSub(p.w, sum.w);
    } else {
      sq = VfSqrt(VfSub(1.0f, VfMul(d, d)));
      th = VfSub(1.0f, __builtin_fabsf(VfAsinQuarter(d)));
      if (!(__builtin_fabsf(VfDaz(d)) < 0.707106769f)) {
        th = VfAsinQuarter(sq);
      }
      wp = VfSinQuarter(VfMul(VfSub(1.0f, weight), th));
      wq = VfSinQuarter(VfMul(weight, th));
      inv = VfRcp(VfSinQuarter(th));
      r.x = VfMul(VfDot2(p.x, q.x, wp, wq), inv);
      r.y = VfMul(VfDot2(p.y, q.y, wp, wq), inv);
      r.z = VfMul(VfDot2(p.z, q.z, wp, wq), inv);
      r.w = VfMul(VfDot2(p.w, q.w, wp, wq), inv);
    }
  } else {
    /* lerp: p + (q - p) * weight */
    r.x = VfAdd(p.x, VfMul(VfSub(q.x, p.x), weight));
    r.y = VfAdd(p.y, VfMul(VfSub(q.y, p.y), weight));
    r.z = VfAdd(p.z, VfMul(VfSub(q.z, p.z), weight));
    r.w = VfAdd(p.w, VfMul(VfSub(q.w, p.w), weight));
  }

  vec[0] = r.x;
  vec[1] = r.y;
  vec[2] = r.z;
  vec[3] = r.w;

  self->flags42 = (self->flags42 & ~(u32)slot[1]) | slot[2];
}
