// bdc 0x0881e23c GfxEffectEmitChildren
#include "bdc.h"

/* Effect command handler that emits child effects: reads a definition id and a shape word from the
   command arguments (`GfxEffectReadIntArg`) and creates the children with `GfxEffectCreate` on
   the same manager. Every child gets the parent's matrix, size, `textureSlot`, `vec1d0`, owner
   (`ownerBakugan`/`ownerId`) and the parent's `color` in its `vec1e0`; bit 0 of the shape adds the
   parent's `dir` (x, y, z) to the child's `dir`, and the child is updated at once (vtable slot 2).
   The children start at the parent's `offset` when it is attached (`attachMatrix`/`attachPos`),
   else at its `pos`. Shapes (`shape & 0xff00`):
   - 0x500: a trail from the previous to the current position (pos - dir when unattached, else the
     position before `GfxEffectUpdateWorldPos`), one child every `spacing / length` of the
     segment (float argument), unattached, `dir` = the segment;
   - 0x300: a ring of children `span / count` radians apart (floats count, radius (+0.01), span,
     0 = 2π) around the effect in its matrix space, `dir` = the normalized radial offset, the angle
     stored in `worldDir[2]`;
   - 0x100 / 0: one child at a random offset in a shell (`GfxEffectRandomPointInShell`) / ring
     (`GfxEffectRandomPointInRing`), floats rMax, rMin, span, transformed by the effect matrix;
     `dir` = the normalized offset (zero when the offset is zero);
   - other: one child at the effect with `dir` = the float vector (x, y, z, 0) transformed by the
     effect matrix.
   Only the single-child shapes stamp the child's `lastTick` with `g_gfxEffectTick`.
   Normalized directions are clamped to [-1, 1] per lane and get `dir[3]` = 0 (bank S713). */

void GfxEffectEmitChildren(GfxEffect *effect, float *args, u16 *cmd)
{
  float vec[4];
  s32 cursor;
  float tmp[4];
  float xf[4];
  float prev[4];
  int id;
  u32 shape;
  u32 kind;
  u32 inherit;
  float *src;
  GfxEffect *child;
  void *owner;
  const VtblEntry *update;
  float x;
  float y;
  float z;
  float count;
  float radius;
  float span;
  float step;
  float end;
  float angle;
  float t;
  float inv;
  float len;
  float k;
  int i;

  vec[0] = 0.0f;
  vec[1] = 0.0f;
  vec[2] = 0.0f;
  vec[3] = 0.0f;
  cursor = 0;
  id = GfxEffectReadIntArg(effect, (s32 *)args, &cursor, cmd);
  shape = (u32)GfxEffectReadIntArg(effect, (s32 *)args, &cursor, cmd);
  kind = shape & 0xff00;
  if (effect->attachMatrix != NULL || effect->attachPos != NULL) {
    src = effect->offset;
  } else {
    src = effect->pos;
  }

  if (kind == 0x500) {
    /* trail between the previous and the current position */
    for (i = 0; i < 4; i++) {
      prev[i] = effect->pos[i];
    }
    inherit = shape & 1;
    if (effect->attachMatrix != NULL || effect->attachPos != NULL) {
      GfxEffectUpdateWorldPos(effect);
      /* vec = pos - prev (vsub.t: w keeps pos[3]) */
      for (i = 0; i < 3; i++) {
        vec[i] = effect->pos[i] - prev[i];
      }
      vec[3] = effect->pos[3];
    } else {
      /* prev = pos - dir (w keeps pos[3]); vec = dir */
      for (i = 0; i < 3; i++) {
        prev[i] = effect->pos[i] - effect->dir[i];
      }
      prev[3] = effect->pos[3];
      for (i = 0; i < 4; i++) {
        vec[i] = effect->dir[i];
      }
    }
    len = __builtin_sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    if (len < 0.01f) {
      inv = 1.0f;
    } else {
      inv = 1.0f / len;
    }
    t = 0.0f;
    step = GfxEffectReadFloatArg(effect, args, &cursor, cmd) * inv;
    do {
      child = (GfxEffect *)GfxEffectCreate(effect->mgr, id);
      /* child->pos = prev + (pos - prev) * t, all four lanes */
      for (i = 0; i < 4; i++) {
        child->pos[i] = prev[i] + (effect->pos[i] - prev[i]) * t;
      }
      child->attachPos = NULL;
      child->attachMatrix = NULL;
      for (i = 0; i < 4; i++) {
        child->dir[i] = vec[i];
      }
      if (inherit != 0) {
        for (i = 0; i < 3; i++) {
          child->dir[i] = child->dir[i] + effect->dir[i];
        }
      }
      __builtin_memcpy(child->matrix, effect->matrix, sizeof(child->matrix));
      for (i = 0; i < 4; i++) {
        child->vec1e0[i] = effect->color[i];
      }
      for (i = 0; i < 4; i++) {
        child->vec1d0[i] = effect->vec1d0[i];
      }
      owner = effect->ownerBakugan;
      child->ownerBakugan = owner;
      if (owner != NULL) {
        child->ownerId = ((CoreObject *)owner)->id;
      }
      child->ownerId = effect->ownerId;
      child->textureSlot = effect->textureSlot;
      for (i = 0; i < 4; i++) {
        child->size[i] = effect->size[i];
      }
      update = &((const VtblEntry *)child->base.vtable)[2];
      ((void (*)(void *))update->fn)((u8 *)child + update->delta);
      t = step + t;
    } while (t < 1.0f);
    return;
  }

  if (kind == 0x300) {
    /* evenly spaced ring around the effect */
    count = GfxEffectReadFloatArg(effect, args, &cursor, cmd);
    radius = GfxEffectReadFloatArg(effect, args, &cursor, cmd) + 0.01f;
    span = GfxEffectReadFloatArg(effect, args, &cursor, cmd);
    if (span == 0.0f) {
      span = 6.2831855f;
    }
    step = span / count;
    angle = 0.0f;
    end = span - 0.0001f;
    inherit = shape & 1;
    for (; angle < end; angle += step) {
      child = (GfxEffect *)GfxEffectCreate(effect->mgr, id);
      for (i = 0; i < 4; i++) {
        child->pos[i] = src[i];
      }
      /* vcos/vsin of angle * 2/π (bank S703): the quarter turns cancel */
      vec[0] = __builtin_cosf(angle) * radius;
      vec[1] = __builtin_sinf(angle) * radius;
      vec[2] = 0.0f;
      vec[3] = 0.0f;
      MathMat4TransformVec4(effect->matrix, xf, vec);
      for (i = 0; i < 4; i++) {
        vec[i] = xf[i];
      }
      for (i = 0; i < 3; i++) {
        child->pos[i] = child->pos[i] + vec[i];
      }
      for (i = 0; i < 4; i++) {
        child->offset[i] = child->pos[i];
      }
      child->attachPos = effect->attachPos;
      child->attachMatrix = effect->attachMatrix;
      /* child->dir = normalize(vec) clamped to [-1, 1], scale 0 (S713) for a zero vector */
      len = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
      k = (len == 0.0f) ? 0.0f : VfRsq(len);
      for (i = 0; i < 3; i++) {
        child->dir[i] = VfSat1(vec[i] * k);
      }
      child->dir[3] = 0.0f;
      if (inherit != 0) {
        for (i = 0; i < 3; i++) {
          child->dir[i] = child->dir[i] + effect->dir[i];
        }
      }
      __builtin_memcpy(child->matrix, effect->matrix, sizeof(child->matrix));
      for (i = 0; i < 4; i++) {
        child->vec1e0[i] = effect->color[i];
      }
      for (i = 0; i < 4; i++) {
        child->vec1d0[i] = effect->vec1d0[i];
      }
      owner = effect->ownerBakugan;
      child->ownerBakugan = owner;
      if (owner != NULL) {
        child->ownerId = ((CoreObject *)owner)->id;
      }
      child->ownerId = effect->ownerId;
      child->textureSlot = effect->textureSlot;
      for (i = 0; i < 4; i++) {
        child->size[i] = effect->size[i];
      }
      child->worldDir[2] = angle;
      update = &((const VtblEntry *)child->base.vtable)[2];
      ((void (*)(void *))update->fn)((u8 *)child + update->delta);
    }
    return;
  }

  inherit = shape & 1;
  if (kind == 0x100 || kind == 0) {
    /* one child at a random point of a shell (0x100) or ring (0) */
    child = (GfxEffect *)GfxEffectCreate(effect->mgr, id);
    for (i = 0; i < 4; i++) {
      child->pos[i] = src[i];
    }
    child->attachPos = effect->attachPos;
    child->attachMatrix = effect->attachMatrix;
    x = GfxEffectReadFloatArg(effect, args, &cursor, cmd);
    y = GfxEffectReadFloatArg(effect, args, &cursor, cmd);
    z = GfxEffectReadFloatArg(effect, args, &cursor, cmd);
    if (kind == 0x100) {
      GfxEffectRandomPointInShell(y, x, z, vec);
    } else {
      GfxEffectRandomPointInRing(y, x, z, vec);
    }
    /* vec = matrix * vec (vtfm4 E: columns are the 16-byte rows of the matrix) */
    for (i = 0; i < 4; i++) {
      tmp[i] = effect->matrix[i] * vec[0] + effect->matrix[4 + i] * vec[1] +
               effect->matrix[8 + i] * vec[2] + effect->matrix[12 + i] * vec[3];
    }
    for (i = 0; i < 4; i++) {
      vec[i] = tmp[i];
    }
    for (i = 0; i < 3; i++) {
      child->pos[i] = child->pos[i] + vec[i];
    }
    for (i = 0; i < 4; i++) {
      child->offset[i] = child->pos[i];
    }
    len = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
    if (len == 0.0f) {
      /* bank C720 = (0, 0, 0, 0) */
      for (i = 0; i < 4; i++) {
        child->dir[i] = 0.0f;
      }
    } else {
      /* child->dir = normalize(vec) clamped to [-1, 1], w = S713 = 0 */
      k = VfRsq(len);
      for (i = 0; i < 3; i++) {
        child->dir[i] = VfSat1(vec[i] * k);
      }
      child->dir[3] = 0.0f;
    }
  } else {
    /* one child at the effect, dir = matrix * (x, y, z, 0) */
    child = (GfxEffect *)GfxEffectCreate(effect->mgr, id);
    for (i = 0; i < 4; i++) {
      child->pos[i] = src[i];
    }
    x = GfxEffectReadFloatArg(effect, args, &cursor, cmd);
    y = GfxEffectReadFloatArg(effect, args, &cursor, cmd);
    z = GfxEffectReadFloatArg(effect, args, &cursor, cmd);
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
    vec[3] = 0.0f;
    for (i = 0; i < 4; i++) {
      tmp[i] = effect->matrix[i] * vec[0] + effect->matrix[4 + i] * vec[1] +
               effect->matrix[8 + i] * vec[2] + effect->matrix[12 + i] * vec[3];
    }
    for (i = 0; i < 4; i++) {
      child->dir[i] = tmp[i];
    }
    child->attachPos = effect->attachPos;
    child->attachMatrix = effect->attachMatrix;
    for (i = 0; i < 4; i++) {
      child->offset[i] = src[i];
    }
  }

  if (inherit != 0) {
    for (i = 0; i < 3; i++) {
      child->dir[i] = child->dir[i] + effect->dir[i];
    }
  }
  __builtin_memcpy(child->matrix, effect->matrix, sizeof(child->matrix));
  child->textureSlot = effect->textureSlot;
  for (i = 0; i < 4; i++) {
    child->size[i] = effect->size[i];
  }
  for (i = 0; i < 4; i++) {
    child->vec1e0[i] = effect->color[i];
  }
  for (i = 0; i < 4; i++) {
    child->vec1d0[i] = effect->vec1d0[i];
  }
  owner = effect->ownerBakugan;
  child->ownerBakugan = owner;
  if (owner != NULL) {
    child->ownerId = ((CoreObject *)owner)->id;
  }
  child->ownerId = effect->ownerId;
  update = &((const VtblEntry *)child->base.vtable)[2];
  ((void (*)(void *))update->fn)((u8 *)child + update->delta);
  child->lastTick = g_gfxEffectTick;
}
