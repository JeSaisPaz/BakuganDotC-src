// bdc 0x089db6c4 GmoDlWriteMeshRenderState
#include "bdc.h"

/* Writes the GE render state of `material` (its `GfxMaterialState` record `info`) into the
   display list `self->cur` and advances it; `GmoDlEmitMesh` calls it with `billboard` false,
   the second pass of `GmoDlDrawMeshes` with `billboard` true.
   Always: culling `0x1d`/front face `0x9b` from `shadeFlags & 3`, stencil test `0xdc` (reference
   `stencilRef`, function `g_gmoStencilWrite`), fog enable `0x1f` from `renderFlags` bits 2..3,
   depth center `0x47` = `g_gfxDepthCenter` + 3 * `depthBias` (+90 for bias 1, which while
   `g_gmoDepthWriteOverride` is set also disables depth writes; the value is kept in
   `g_gmoMeshDepthCenter`), stencil enable/op `0x24`/`0xdd` from `g_gmoStencilWrite`, and
   lighting off (`0x17`, `0x90ffffff`) for `renderFlags` bit 3.
   First pass: alpha test `0x22`/`0xdb` (GREATER than `alphaRef` when it is nonzero and below
   `g_gfxMaterialAlpha`, else than 1), blending `0xdf`/`0xe0`/`0xe1` from `renderFlags` bits 6..7
   (none, alpha, additive, subtractive; the last two also write fog colour `0xcf000000` and disable
   depth writes), depth mask `0xe7` (also off for `renderFlags & 0x10`), the material texture's GE
   state block `texSlot - 1` (`GfxTextureSelectSlot`, `GfxTextureWriteCall`) and the
   `animCallback` (called with the address of the list cursor and `animArg`, or
   `g_gfxCurrentModel` when that is NULL). Returns 1 when the callback ran, else 0.
   Billboard pass: stencil test EQUAL, then either (shadeFlags bits 2..4 zero) an environment-map
   texture matrix (`0x40` + 12 `0x41` words) = the camera's `viewOrtho` x the orthonormalised
   `model->rootMatrix` x `node->localMatrix` (the asm spills bank column C730 to a dead stack slot),
   state list B slot `(shadeFlags >> 5) + 6` (`GmoDlCallStateListB`), blending from `renderFlags`
   bits 0..1 and, for shadeFlags bits 5..7 = 6, front face 1, `0xde000007` and stencil ALWAYS; or
   state list A slot `((shadeFlags >> 2) & 7) - 1` (`GmoDlCallStateListA`) with alpha blending.
   Then material alpha `0x58` (`g_gfxMaterialAlpha`) and diffuse white `0x56ffffff`; returns 2. */

typedef void (*GmoMaterialAnimFn)(u32 **list, void *arg);

/* Column `col` of `a * b` (`vmmul.q`): the sum over k of col[k] * column k of `a`. */
static void GmoDlMat4MulColumn(ScePspFVector4 *d, const ScePspFMatrix4 *a, const ScePspFVector4 *col)
{
  d->x = col->x * a->x.x + col->y * a->y.x + col->z * a->z.x + col->w * a->w.x;
  d->y = col->x * a->x.y + col->y * a->y.y + col->z * a->z.y + col->w * a->w.y;
  d->z = col->x * a->x.z + col->y * a->y.z + col->z * a->z.z + col->w * a->w.z;
  d->w = col->x * a->x.w + col->y * a->y.w + col->z * a->z.w + col->w * a->w.w;
}

/* `out = a * b` in the engine's column-major layout (`vmmul.q M000, M100, M200`); `out` must not
   alias `a` or `b`. */
static void GmoDlMat4Mul(ScePspFMatrix4 *out, const ScePspFMatrix4 *a, const ScePspFMatrix4 *b)
{
  GmoDlMat4MulColumn(&out->x, a, &b->x);
  GmoDlMat4MulColumn(&out->y, a, &b->y);
  GmoDlMat4MulColumn(&out->z, a, &b->z);
  GmoDlMat4MulColumn(&out->w, a, &b->w);
}

u32 GmoDlWriteMeshRenderState(GmoDlContext *self, GmoMaterial *material, GmoModel *model, GmoNode *node, bool billboard)

{
  GfxMaterialState *state = (GfxMaterialState *)material->info;
  u32 *dl = self->cur;
  u32 *cursor;
  u32 result = 0;
  u32 depthMask = 0;
  u8 depthOverride;
  float depth;
  union { float f; u32 u; } bits;

  if ((state->shadeFlags & 3) == 0) {
    dl[0] = 0x1d000000;
    dl[1] = 0;
  }
  else if ((state->shadeFlags & 3) == 1) {
    dl[0] = 0x1d000001;
    dl[1] = 0x9b000000;
  }
  else {
    dl[0] = 0x1d000001;
    dl[1] = 0x9b000001;
  }
  dl[2] = (u32)state->stencilRef << 8 | 0xdcff0000 | g_gmoStencilWrite;
  dl[3] = (u32)(state->renderFlags & 0xc) >> 2 | 0x1f000000;
  depthOverride = g_gmoDepthWriteOverride;
  depth = g_gfxDepthCenter;
  if (state->depthBias == 1) {
    if (depthOverride != 0) {
      depthMask = 1;
    }
    depth = depth + 90.0f;
  }
  else {
    depth = depth + (float)state->depthBias * 3.0f;
  }
  bits.f = depth;
  dl[4] = bits.u >> 8 | 0x47000000;
  g_gmoMeshDepthCenter = depth;
  if (g_gmoStencilWrite == 0) {
    dl[5] = 0x24000000;
    dl[6] = 0xdd000000;
  }
  else {
    dl[5] = 0x24000001;
    dl[6] = 0xdd020000;
  }
  if (((u32)(state->renderFlags & 0xc) >> 2 & 2) != 0) {
    dl[7] = 0x17000000;
    dl[8] = 0x90ffffff;
    cursor = dl + 9;
  }
  else {
    cursor = dl + 7;
  }
  if ((state->renderFlags & 0x10) != 0) {
    depthMask = 1;
  }

  if (!billboard) {
    u32 alphaTest;
    u32 blend;

    cursor[0] = 0x22000001;
    alphaTest = 0xdbff0106;
    if (state->alphaRef != 0 && (u32)(s16)state->alphaRef < g_gfxMaterialAlpha) {
      alphaTest = (u32)(s16)state->alphaRef << 8 | 0xdbff0006;
    }
    cursor[1] = alphaTest;
    cursor += 2;
    switch ((u32)(state->renderFlags & 0xc0) >> 6) {
    case 0:
      cursor[0] = 0xdf0000aa;
      cursor[1] = 0xe0ffffff;
      cursor[2] = 0xe1000000;
      cursor += 3;
      break;
    case 1:
      cursor[0] = 0xdf000032;
      cursor[1] = 0xe0000000;
      cursor[2] = 0xe1000000;
      cursor += 3;
      break;
    case 2:
      cursor[0] = 0xdf0000a2;
      cursor[1] = 0xe0000000;
      cursor[2] = 0xe1ffffff;
      cursor += 3;
      break;
    case 3:
      cursor[0] = 0xdf0002a2;
      cursor[1] = 0xe0000000;
      cursor[2] = 0xe1ffffff;
      cursor += 3;
      break;
    }
    blend = (u32)(state->renderFlags & 0xc0) >> 6;
    if (blend >= 2) {
      *cursor++ = 0xcf000000;
      depthMask = 1;
    }
    *cursor++ = depthMask | 0xe7000000;
    if (state->texSlot != 0) {
      s32 slot = state->texSlot - 1;
      GmoTexture *texture =
          ((GmoLayer *)model->textures)[material->attrs->layerRef & 0x3ff].texture;
      GfxTextureSelectSlot(texture->palette, slot);
      cursor = GfxTextureWriteCall(texture->palette, cursor, slot);
    }
    if (state->animCallback != NULL) {
      if (state->animArg == NULL) {
        ((GmoMaterialAnimFn)state->animCallback)(&cursor, g_gfxCurrentModel);
      }
      else {
        ((GmoMaterialAnimFn)state->animCallback)(&cursor, state->animArg);
      }
      result = 1;
    }
    self->cur = cursor;
    return result;
  }

  *cursor++ = (u32)state->stencilRef << 8 | 0xdcff0002;
  if ((state->shadeFlags & 0x1c) == 0) {
    union {
      ScePspFMatrix4 m;
      u32 u[16];
    } texMtx;
    ScePspFMatrix4 tmp;
    ScePspFVector4 x;
    ScePspFVector4 z;
    float kx;
    float ky;
    float kz;
    u32 cmd;
    int row;

    /* texMtx = rootMatrix * localMatrix (`vmmul.q`). */
    GmoDlMat4Mul(&texMtx.m, (const ScePspFMatrix4 *)model->rootMatrix,
                 (const ScePspFMatrix4 *)node->localMatrix);
    /* Re-orthonormalise the columns: z = x × y, x = y × z (`vcrsp.t`, `w` zeroed by `vzero.t R003`),
       each of x, y, z scaled by 1/|v| (`vdot.t`, `vrsq.t`, `vscl.t`); y keeps its `w`. */
    tmp = texMtx.m;
    z.x = tmp.x.y * tmp.y.z - tmp.x.z * tmp.y.y;
    z.y = tmp.x.z * tmp.y.x - tmp.x.x * tmp.y.z;
    z.z = tmp.x.x * tmp.y.y - tmp.x.y * tmp.y.x;
    z.w = 0.0f;
    x.x = tmp.y.y * z.z - tmp.y.z * z.y;
    x.y = tmp.y.z * z.x - tmp.y.x * z.z;
    x.z = tmp.y.x * z.y - tmp.y.y * z.x;
    x.w = 0.0f;
    kx = VfRsq(x.x * x.x + x.y * x.y + x.z * x.z);
    ky = VfRsq(tmp.y.x * tmp.y.x + tmp.y.y * tmp.y.y + tmp.y.z * tmp.y.z);
    kz = VfRsq(z.x * z.x + z.y * z.y + z.z * z.z);
    texMtx.m.x.x = x.x * kx;
    texMtx.m.x.y = x.y * kx;
    texMtx.m.x.z = x.z * kx;
    texMtx.m.x.w = x.w;
    texMtx.m.y.x = tmp.y.x * ky;
    texMtx.m.y.y = tmp.y.y * ky;
    texMtx.m.y.z = tmp.y.z * ky;
    texMtx.m.z.x = z.x * kz;
    texMtx.m.z.y = z.y * kz;
    texMtx.m.z.z = z.z * kz;
    texMtx.m.z.w = z.w;
    /* texMtx = camera->viewOrtho * texMtx. (The asm also spills the bank column C730 = (0, 0, 0, 1)
       to a stack slot nothing reads.) */
    tmp = texMtx.m;
    GmoDlMat4Mul(&texMtx.m, &g_gfxActiveCamera->viewOrtho, &tmp);

    /* TMATRIX words: the template's command byte with the float's upper 24 bits (`lwr`). */
    cursor[0] = 0x40000000;
    cmd = g_gmoTexMatrixCmd & 0xff000000;
    for (row = 0; row < 4; row++) {
      cursor[1 + row * 3] = cmd | texMtx.u[row * 4] >> 8;
      cursor[2 + row * 3] = cmd | texMtx.u[row * 4 + 1] >> 8;
      cursor[3 + row * 3] = cmd | texMtx.u[row * 4 + 2] >> 8;
    }
    cursor = GmoDlCallStateListB(cursor + 13, ((u32)(state->shadeFlags & 0xe0) >> 5) + 6);
    switch (state->renderFlags & 3) {
    case 0:
      cursor[0] = 0xdf0000aa;
      cursor[1] = 0xe0ffffff;
      cursor[2] = 0xe1000000;
      cursor += 3;
      break;
    case 1:
      cursor[0] = 0xdf000032;
      cursor[1] = 0xe0000000;
      cursor[2] = 0xe1000000;
      cursor += 3;
      break;
    case 2:
      cursor[0] = 0xdf0000a2;
      cursor[1] = 0xe0000000;
      cursor[2] = 0xe1ffffff;
      cursor += 3;
      break;
    case 3:
      cursor[0] = 0xdf0002a2;
      cursor[1] = 0xe0000000;
      cursor[2] = 0xe1ffffff;
      cursor += 3;
      break;
    }
    if ((state->shadeFlags & 0xe0) == 0xc0) {
      cursor[0] = 0x9b000001;
      cursor[1] = 0xde000007;
      cursor[2] = 0xdcff0001;
      cursor += 3;
    }
  }
  else {
    cursor = GmoDlCallStateListA(cursor, ((u32)(state->shadeFlags & 0x1c) >> 2) - 1);
    cursor[0] = 0xdf000032;
    cursor[1] = 0xe0000000;
    cursor[2] = 0xe1000000;
    cursor += 3;
  }
  cursor[0] = g_gfxMaterialAlpha | 0x58000000;
  cursor[1] = 0x56ffffff;
  self->cur = cursor + 2;
  return 2;
}
