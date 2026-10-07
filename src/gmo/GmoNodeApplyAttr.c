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
  vec = (float *)((u8 *)self + slot[0]);

  q.x = value[0];
  q.y = value[1];
  q.z = value[2];
  q.w = value[3];

  if ((u32)(type - 0x49) < 2) {
    /* Euler angles (radians) -> quaternion: angle * (1/pi) quarter turns is the half angle */
    q.x = q.x * 0.318309873f;
    q.y = q.y * 0.318309873f;
    q.z = q.z * 0.318309873f;
    cx = VfCosQuarter(q.x);
    sx = VfSinQuarter(q.x);
    cy = VfCosQuarter(q.y);
    sy = VfSinQuarter(q.y);
    cz = VfCosQuarter(q.z);
    sz = VfSinQuarter(q.z);
    cxy = cx * cy;
    sxy = sx * sy;
    cyz = cy * cz;
    czx = cz * cx;
    syz = sy * sz;
    szx = sz * sx;
    cxyz = cxy * cz;
    sxyz = sxy * sz;
    if (type == 0x49) {
      q.x = cyz * sx - syz * cx;
      q.y = czx * sy + szx * cy;
      q.z = cxy * sz - sxy * cz;
      q.w = cxyz + sxyz;
    } else {
      q.x = cyz * sx + syz * cx;
      q.y = czx * sy - szx * cy;
      q.z = cxy * sz - sxy * cz;
      q.w = cxyz + sxyz;
    }
  }

  p.x = vec[0];
  p.y = vec[1];
  p.z = vec[2];
  p.w = vec[3];

  if ((u32)(type - 0x49) < 3) {
    /* slerp from p toward q by weight */
    d = VfSat1(p.x * q.x + p.y * q.y + p.z * q.z + p.w * q.w);
    diff.x = (q.x - p.x) * weight;
    diff.y = (q.y - p.y) * weight;
    diff.z = (q.z - p.z) * weight;
    diff.w = (q.w - p.w) * weight;
    sum.x = (q.x + p.x) * weight;
    sum.y = (q.y + p.y) * weight;
    sum.z = (q.z + p.z) * weight;
    sum.w = (q.w + p.w) * weight;
    if (d < 0.0f) {
      q.x = -q.x;
      q.y = -q.y;
      q.z = -q.z;
      q.w = -q.w;
    }
    if (!(d < 0.998046875f)) {
      r.x = p.x + diff.x;
      r.y = p.y + diff.y;
      r.z = p.z + diff.z;
      r.w = p.w + diff.w;
    } else if (d < -0.998046875f) {
      r.x = p.x - sum.x;
      r.y = p.y - sum.y;
      r.z = p.z - sum.z;
      r.w = p.w - sum.w;
    } else {
      sq = __builtin_sqrtf(1.0f - d * d);
      th = 1.0f - __builtin_fabsf(VfAsinQuarter(d));
      if (!(__builtin_fabsf(d) < 0.707106769f)) {
        th = VfAsinQuarter(sq);
      }
      wp = VfSinQuarter((1.0f - weight) * th);
      wq = VfSinQuarter(weight * th);
      inv = VfRcp(VfSinQuarter(th));
      r.x = (p.x * wp + q.x * wq) * inv;
      r.y = (p.y * wp + q.y * wq) * inv;
      r.z = (p.z * wp + q.z * wq) * inv;
      r.w = (p.w * wp + q.w * wq) * inv;
    }
  } else {
    /* lerp: p + (q - p) * weight */
    r.x = p.x + (q.x - p.x) * weight;
    r.y = p.y + (q.y - p.y) * weight;
    r.z = p.z + (q.z - p.z) * weight;
    r.w = p.w + (q.w - p.w) * weight;
  }

  vec[0] = r.x;
  vec[1] = r.y;
  vec[2] = r.z;
  vec[3] = r.w;

  self->flags42 = (self->flags42 & ~(u32)slot[1]) | slot[2];
}
