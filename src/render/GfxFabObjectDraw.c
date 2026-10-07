// bdc 0x08a01b20 GfxFabObjectDraw
#include "bdc.h"

/* Draws one placed object of a `.fab` 2D animation clip, mutually recursive with
   `GfxFabClipDraw`: combines the object's 4x4 transform with `parentMtx` (the VFPU
   `vmmul.q M000, M100, M200`: element `[c][r]` of the result is the dot product of row `r` of
   `parentMtx` with row `c` of `transform`, as stored), multiplies its colour by
   `color` and adds `colorAdd` to its additive colour. A nested clip (definition kind `ref[6]` 2)
   returns `GfxFabClipDraw` of `subClip` with the combined matrix and colours. Otherwise it
   emits into `list`: the world matrix (WORLDMATRIXNUMBER 0 + 12 WORLDMATRIXDATA words), material
   ambient colour/alpha from the combined colour and the specular-slot (`0x57`) colour from the
   combined additive colour (each lane clamped to [0, 1], scaled by 255 and truncated to a byte), the blend state for
   `blendMode` (-1/1 alpha, 0 opaque, 2 additive, 3 subtractive, other values none), the texture
   call (`GfxTextureWriteCall` slot 0), linear filtering, and a 4-vertex triangle strip: the
   shared `g_gfxFabDefaultQuad` or a copy of the object's own 48-byte vertices inlined into the
   list behind a JUMP. Returns the advanced list pointer. */

u32 *GfxFabObjectDraw(void *obj, u32 *list, const float *parentMtx, void *color, void *colorAdd)
{
  GfxFabObject *o = (GfxFabObject *)obj;
  union {
    float f[16];
    u32 u[16];
  } mtx;
  float col[4];
  float colAdd[4];
  const float *c = (const float *)color;
  const float *ca = (const float *)colorAdd;
  u32 packed;
  u32 *verts;
  u32 *after;
  const u32 *src;
  const void *uvs;
  int i, k;

  /* vmmul.q M000, M100 (parentMtx), M200 (transform) */
  for (i = 0; i < 4; i++) {
    for (k = 0; k < 4; k++) {
      mtx.f[i * 4 + k] = parentMtx[k * 4 + 0] * o->transform[i][0] + parentMtx[k * 4 + 1] * o->transform[i][1] +
                         parentMtx[k * 4 + 2] * o->transform[i][2] + parentMtx[k * 4 + 3] * o->transform[i][3];
    }
  }
  for (i = 0; i < 4; i++) {
    col[i] = o->color[i] * c[i];
  }
  for (i = 0; i < 4; i++) {
    colAdd[i] = o->colorAdd[i] + ca[i];
  }

  if (o->ref[6] == 2) {
    return GfxFabClipDraw(o->subClip, list, mtx.f, col, colAdd);
  }

  /* WORLDMATRIXNUMBER 0, then the 4x3 part of `mtx` as WORLDMATRIXDATA (24-bit floats) */
  list[0] = 0x3a000000;
  list[1] = g_geWorldMatrixDataCmd | (mtx.u[0] >> 8);
  list[2] = g_geWorldMatrixDataCmd | (mtx.u[1] >> 8);
  list[3] = g_geWorldMatrixDataCmd | (mtx.u[2] >> 8);
  list[4] = g_geWorldMatrixDataCmd | (mtx.u[4] >> 8);
  list[5] = g_geWorldMatrixDataCmd | (mtx.u[5] >> 8);
  list[6] = g_geWorldMatrixDataCmd | (mtx.u[6] >> 8);
  list[7] = g_geWorldMatrixDataCmd | (mtx.u[8] >> 8);
  list[8] = g_geWorldMatrixDataCmd | (mtx.u[9] >> 8);
  list[9] = g_geWorldMatrixDataCmd | (mtx.u[10] >> 8);
  list[10] = g_geWorldMatrixDataCmd | (mtx.u[12] >> 8);
  list[11] = g_geWorldMatrixDataCmd | (mtx.u[13] >> 8);
  list[12] = g_geWorldMatrixDataCmd | (mtx.u[14] >> 8);
  list += 13;

  /* vsat0, vscl by S701 (255), vf2iz 23, vi2uc: lane 0 in the low byte */
  packed = 0;
  for (i = 0; i < 4; i++) {
    packed |= (u32)VfI2uc(VfF2iz(VfSat0(col[i]) * 255.0f, 23)) << (i * 8);
  }
  list[0] = (packed & 0xffffff) | 0x55000000; /* AMBIENTCOLOR */
  list[1] = (packed >> 24) | 0x58000000;      /* AMBIENTALPHA */
  list += 2;

  packed = 0;
  for (i = 0; i < 4; i++) {
    packed |= (u32)VfI2uc(VfF2iz(VfSat0(colAdd[i]) * 255.0f, 23)) << (i * 8);
  }
  list[0] = (packed & 0xffffff) | 0x57000000;
  list += 1;

  /* ALPHABLEND / FIXA / FIXB */
  switch (o->blendMode) {
  case -1:
  case 1:
    list[0] = 0xdf000032;
    list[1] = 0xe0000000;
    list[2] = 0xe1000000;
    list += 3;
    break;
  case 0:
    list[0] = 0xdf0000aa;
    list[1] = 0xe0ffffff;
    list[2] = 0xe1000000;
    list += 3;
    break;
  case 2:
    list[0] = 0xdf0000a2;
    list[1] = 0xe0000000;
    list[2] = 0xe1ffffff;
    list += 3;
    break;
  case 3:
    list[0] = 0xdf0002a2;
    list[1] = 0xe0000000;
    list[2] = 0xe1ffffff;
    list += 3;
    break;
  default:
    break;
  }

  list = GfxTextureWriteCall(o->bitmap, list, 0);
  *list++ = 0xc6000101; /* TFILTER linear/linear */

  if (o->uvs == (void *)g_gfxFabDefaultQuad) {
    uvs = o->uvs;
    *list++ = 0x12000081; /* VTYPE: u8 UV, s8 position */
    if (uvs != 0) {
      list[0] = 0x10000000 | ((((uintptr_t)uvs >> 24) & 0xf) << 16); /* BASE */
      list[1] = 0x01000000 | ((uintptr_t)uvs & 0xffffff);             /* VADDR */
      list += 2;
    }
    *list++ = 0x04040004; /* PRIM triangle strip, 4 vertices */
    return list;
  }

  /* inline the 48 bytes of vertices (16-byte aligned) and JUMP over them */
  verts = (u32 *)(((uintptr_t)(list + 2) + 0xf) & ~(uintptr_t)0xf);
  after = verts + 12;
  list[0] = 0x10000000 | ((((uintptr_t)after >> 24) & 0xf) << 16); /* BASE */
  list[1] = 0x08000000 | ((uintptr_t)after & 0xffffff);             /* JUMP */
  src = (const u32 *)o->uvs;
  for (i = 0; i < 3; i++) {
    verts[i * 4 + 0] = src[i * 4 + 0];
    verts[i * 4 + 1] = src[i * 4 + 1];
    verts[i * 4 + 2] = src[i * 4 + 2];
    verts[i * 4 + 3] = src[i * 4 + 3];
  }
  *after = 0x12000083; /* VTYPE: float UV, s8 position */
  list = after + 1;
  if (verts != 0) {
    list[0] = 0x10000000 | ((((uintptr_t)verts >> 24) & 0xf) << 16); /* BASE */
    list[1] = 0x01000000 | ((uintptr_t)verts & 0xffffff);             /* VADDR */
    list += 2;
  }
  *list++ = 0x04040004; /* PRIM triangle strip, 4 vertices */
  return list;
}
