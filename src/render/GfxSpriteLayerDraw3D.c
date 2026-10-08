// bdc 0x089f60d8 GfxSpriteLayerDraw3D
#include "bdc.h"

/* Renders a 3D (billboard) sprite layer, the body of `GfxSpriteLayerDrawWorld`: opens a packet
   chunk (`GfxPacketBeginChunk`), calls the 2D render state (`GfxDlCall2DState`) and emits
   linear filtering, `0x24 1`, `0x36 0x2040`, `0x5e 0`, depth test (`g_gfxSpriteDepthTest`) and
   fog enable (`g_gfxSpriteFogEnable`), loads the camera (`GfxCameraDlWrite`, all parts),
   collects the visible sprites with `GfxSpriteLayerCollect3D` into `g_gfxSpriteSortPairs`
   (at most `maxSorted`, sorted with `GfxCombSortByDepth` when `sorted` is set) and emits each:
   a texture call when texture or slot changed (`GfxTextureWriteCall`), the stencil words
   (`GfxSpriteWriteStencilState`), UV offset/scale (`0x4a/0x4b/0x48/0x49`, float bits >> 8), a
   world matrix built per `billboardMode` with the VFPU (`GfxDlWriteSpriteWorldMatrix`, position
   relative to `origin` when it is non-NULL), the blend state when `blendMode & 0xffff` changed (0
   source replaces (FIXA white, FIXB black), 1 src-alpha / 1-src-alpha, 2 additive (src alpha + FIXB
   white), 3 reverse subtract dst - src*alpha), the tint/alpha colour words `0x55`/`0x58`, then
   either a Bezier patch (`GfxDlWriteBezierScrollPatch`, flags bit `0x10000000`) or the quad's
   vertices (quadMode < 2: VTYPE `0x81` + VADDR of `vertices`; otherwise the 48-byte vertex block is
   copied inline into the display list and jumped over, VTYPE `0x83`). Closes the chunk with
   `GfxPacketEndChunk`.
   billboardMode: 0 = camera billboard matrix times (size scale x Z rotation by `+0x88`), 1 = the
   sprite matrix as is, 2 = sprite matrix rows scaled by width/height/depth, 3 = rotation from the
   quaternion `+0xa0`, 4 = basis facing `toCamera` around the axis `+0x80` (flags `0x10`) or `+0x90`,
   scaled by the size, 10 = like 0 against the sprite matrix; other modes reuse whatever the local
   matrix held (the previous sprite's, or uninitialised stack for the first one).
   The VFPU bank constants it reads are literals here: S703 (2/pi, so `vrot` of angle * S703 is
   cosf/sinf of the angle), S713 (0, the inverse length used for a zero-length vector) and S701 (255,
   the tint scale). */

void GfxSpriteLayerDraw3D(GfxSpriteLayer *self, RenderPacket *packet, GfxCamera *camera, const float *origin)

{
  GfxSprite *head;
  GfxSprite *sprite;
  GfxSpriteSortPair *pair;
  u32 *dl;
  u32 *buf;
  u32 *jump;
  u32 *p;
  const float *world;
  const float *basis;
  GfxSpriteVertex *verts;
  void *texture;
  s32 slot;
  s32 blend;
  s32 count;
  s32 i;
  s32 j;
  s32 r;
  s32 k;
  u32 rgba;
  float angle;
  float c;
  float s;
  float lenSq;
  float inv;
  float sum;
  float axis[3];
  float vx[3];
  float vy[3];
  float vz[3];
  float lane[4];
  float qa[4][4];
  float qb[4][4];
  float local[16];
  union { float f; u32 u; } bits0, bits1;
  float m[16];
  float relPos[4];

  head = self->head;
  dl = GfxPacketBeginChunk(packet);
  dl = GfxDlCall2DState(dl);
  dl[0] = 0xc6000101;
  dl[1] = 0x24000001;
  dl[2] = 0x36002040;
  dl[3] = 0x5e000000;
  dl[4] = g_gfxSpriteDepthTest | 0x23000000;
  dl[5] = g_gfxSpriteFogEnable | 0x1f000000;
  dl = GfxCameraDlWrite(camera, dl + 6, 0xffffffff);
  texture = NULL;
  blend = -1;
  slot = -1;
  count = GfxSpriteLayerCollect3D(head, g_gfxSpriteSortPairs, camera, self->maxSorted);
  if (self->sorted != 0) {
    GfxCombSortByDepth(g_gfxSpriteSortPairs, count);
  }
  for (i = 0, pair = g_gfxSpriteSortPairs; i < count; i++, pair++) {
    sprite = pair->sprite;
    if (texture != sprite->texture || slot != sprite->textureSlot) {
      texture = sprite->texture;
      slot = sprite->textureSlot;
      dl = GfxTextureWriteCall(texture, dl, slot);
    }
    dl = GfxSpriteWriteStencilState(sprite, dl);
    bits0.f = sprite->uvOffsetU;
    bits1.f = sprite->uvOffsetV;
    dl[0] = bits0.u >> 8 | 0x4a000000;
    dl[1] = bits1.u >> 8 | 0x4b000000;
    bits0.f = sprite->uvScaleU;
    bits1.f = sprite->uvScaleV;
    dl[2] = bits0.u >> 8 | 0x48000000;
    dl[3] = bits1.u >> 8 | 0x49000000;
    dl += 4;

    world = m;
    switch ((u32)sprite->billboardMode) {
    case 0:
    case 10:
      /* local = rotZ(angle) * scale(width, height, depth): row j of local is rotation column j
         times size j (vrot of angle * 2/pi is cos/sin of the angle). Then
         m[j][r] = dot(local row j, basis row r), basis = the camera billboard (0) or the sprite
         matrix (10). */
      angle = sprite->maybe_billboardParams80[2];
      c = __builtin_cosf(angle);
      s = __builtin_sinf(angle);
      local[0] = c * sprite->width;
      local[1] = s * sprite->width;
      local[2] = 0.0f;
      local[3] = 0.0f;
      local[4] = -s * sprite->height;
      local[5] = c * sprite->height;
      local[6] = 0.0f;
      local[7] = 0.0f;
      local[8] = 0.0f;
      local[9] = 0.0f;
      local[10] = sprite->depth;
      local[11] = 0.0f;
      local[12] = 0.0f;
      local[13] = 0.0f;
      local[14] = 0.0f;
      local[15] = 1.0f;
      if (sprite->billboardMode == 0) {
        basis = (const float *)&camera->billboard;
      } else {
        basis = sprite->matrix;
      }
      for (j = 0; j < 4; j++) {
        for (r = 0; r < 4; r++) {
          sum = local[j * 4] * basis[r * 4];
          for (k = 1; k < 4; k++) {
            sum = sum + local[j * 4 + k] * basis[r * 4 + k];
          }
          m[j * 4 + r] = sum;
        }
      }
      break;
    case 1:
      world = sprite->matrix;
      break;
    case 2:
      /* sprite matrix rows 0-2 scaled by width/height/depth (row 3 of m left as it was) */
      for (k = 0; k < 4; k++) {
        m[k] = sprite->matrix[k] * sprite->width;
        m[4 + k] = sprite->matrix[4 + k] * sprite->height;
        m[8 + k] = sprite->matrix[8 + k] * sprite->depth;
      }
      break;
    case 3:
      /* 3x3 of the product of the two sign/swizzle matrices of the quaternion q = +0xa0 (vpfxs
         columns qa[k], qb[k]): m[c][r] = sum_k qa[k][r] * qb[k][c]; row/column 3 identity. */
      lane[0] = sprite->maybe_billboardParamsA0[0];
      lane[1] = sprite->maybe_billboardParamsA0[1];
      lane[2] = sprite->maybe_billboardParamsA0[2];
      lane[3] = sprite->maybe_billboardParamsA0[3];
      qa[0][0] = lane[3];  qa[0][1] = lane[2];  qa[0][2] = -lane[1]; qa[0][3] = -lane[0];
      qa[1][0] = -lane[2]; qa[1][1] = lane[3];  qa[1][2] = lane[0];  qa[1][3] = -lane[1];
      qa[2][0] = lane[1];  qa[2][1] = -lane[0]; qa[2][2] = lane[3];  qa[2][3] = -lane[2];
      qa[3][0] = lane[0];  qa[3][1] = lane[1];  qa[3][2] = lane[2];  qa[3][3] = lane[3];
      qb[0][0] = lane[3];  qb[0][1] = lane[2];  qb[0][2] = -lane[1]; qb[0][3] = lane[0];
      qb[1][0] = -lane[2]; qb[1][1] = lane[3];  qb[1][2] = lane[0];  qb[1][3] = lane[1];
      qb[2][0] = lane[1];  qb[2][1] = -lane[0]; qb[2][2] = lane[3];  qb[2][3] = lane[2];
      qb[3][0] = -lane[0]; qb[3][1] = -lane[1]; qb[3][2] = -lane[2]; qb[3][3] = lane[3];
      for (j = 0; j < 3; j++) {
        for (r = 0; r < 3; r++) {
          sum = qa[0][r] * qb[0][j];
          for (k = 1; k < 4; k++) {
            sum = sum + qa[k][r] * qb[k][j];
          }
          m[j * 4 + r] = sum;
        }
        m[j * 4 + 3] = 0.0f;
      }
      m[12] = 0.0f;
      m[13] = 0.0f;
      m[14] = 0.0f;
      m[15] = 1.0f;
      break;
    case 4:
      /* z = normalize(toCamera), x = normalize(axis x z), y = z x x (each normalised lane clamped
         to [-1, 1]; a zero length gives a zero vector); rows scaled by the size. The binary has one
         copy of this block per axis; they differ only in the axis address. */
      if ((sprite->flags & 0x10) != 0) {
        axis[0] = sprite->maybe_billboardParams80[0];
        axis[1] = sprite->maybe_billboardParams80[1];
        axis[2] = sprite->maybe_billboardParams80[2];
      } else {
        axis[0] = sprite->scaleX;
        axis[1] = sprite->scaleY;
        axis[2] = sprite->scaleZ;
      }
      vz[0] = sprite->toCamera[0];
      vz[1] = sprite->toCamera[1];
      vz[2] = sprite->toCamera[2];
      lenSq = vz[0] * vz[0] + vz[1] * vz[1] + vz[2] * vz[2];
      inv = lenSq == 0.0f ? 0.0f : VfRsq(lenSq);
      vz[0] = VfSat1(vz[0] * inv);
      vz[1] = VfSat1(vz[1] * inv);
      vz[2] = VfSat1(vz[2] * inv);
      vx[0] = axis[1] * vz[2] - axis[2] * vz[1];
      vx[1] = axis[2] * vz[0] - axis[0] * vz[2];
      vx[2] = axis[0] * vz[1] - axis[1] * vz[0];
      lenSq = vx[0] * vx[0] + vx[1] * vx[1] + vx[2] * vx[2];
      inv = lenSq == 0.0f ? 0.0f : VfRsq(lenSq);
      vx[0] = VfSat1(vx[0] * inv);
      vx[1] = VfSat1(vx[1] * inv);
      vx[2] = VfSat1(vx[2] * inv);
      vy[0] = vz[1] * vx[2] - vz[2] * vx[1];
      vy[1] = vz[2] * vx[0] - vz[0] * vx[2];
      vy[2] = vz[0] * vx[1] - vz[1] * vx[0];
      for (k = 0; k < 3; k++) {
        m[k] = vx[k] * sprite->width;
        m[4 + k] = vy[k] * sprite->height;
        m[8 + k] = vz[k] * sprite->depth;
      }
      m[3] = 0.0f * sprite->width;
      m[7] = 0.0f * sprite->height;
      m[11] = 0.0f * sprite->depth;
      m[12] = 0.0f;
      m[13] = 0.0f;
      m[14] = 0.0f;
      m[15] = 1.0f;
      break;
    default:
      /* 5-9 and > 10: m is not rebuilt */
      break;
    }

    if (origin != NULL) {
      /* relPos = pos - origin (xyz; w stays pos.w) */
      relPos[0] = sprite->posX - origin[0];
      relPos[1] = sprite->posY - origin[1];
      relPos[2] = sprite->posZ - origin[2];
      relPos[3] = sprite->posW;
      dl = GfxDlWriteSpriteWorldMatrix(dl, world, relPos);
    } else {
      dl = GfxDlWriteSpriteWorldMatrix(dl, world, &sprite->posX);
    }

    if (blend != (s32)(sprite->blendMode & 0xffff)) {
      blend = (s32)(sprite->blendMode & 0xffff);
      switch (blend) {
      case 0:
        dl[0] = 0xdf0000aa;
        dl[1] = 0xe0ffffff;
        dl[2] = 0xe1000000;
        dl += 3;
        break;
      case 1:
        dl[0] = 0xdf000032;
        dl[1] = 0xe0000000;
        dl[2] = 0xe1000000;
        dl += 3;
        break;
      case 2:
        dl[0] = 0xdf0000a2;
        dl[1] = 0xe0000000;
        dl[2] = 0xe1ffffff;
        dl += 3;
        break;
      case 3:
        dl[0] = 0xdf0002a2;
        dl[1] = 0xe0000000;
        dl[2] = 0xe1ffffff;
        dl += 3;
        break;
      }
    }

    /* tint rgb + alpha saturated to [0,1], scaled by 255 and packed to 4 bytes (r in the low byte) */
    rgba = (u32)VfI2uc(VfF2iz(VfSat0(sprite->tint[0]) * 255.0f, 23)) |
           (u32)VfI2uc(VfF2iz(VfSat0(sprite->tint[1]) * 255.0f, 23)) << 8 |
           (u32)VfI2uc(VfF2iz(VfSat0(sprite->tint[2]) * 255.0f, 23)) << 16 |
           (u32)VfI2uc(VfF2iz(VfSat0(sprite->alpha) * 255.0f, 23)) << 24;
    dl[0] = (rgba & 0xffffff) | 0x55000000;
    dl[1] = rgba >> 24 | 0x58000000;
    dl += 2;

    if ((sprite->flags & 0x10000000) != 0) {
      dl = GfxDlWriteBezierScrollPatch(sprite->patch.scaleU, dl, sprite->patch.divS,
                                       sprite->patch.divT);
    } else if (sprite->quadMode < 2) {
      verts = sprite->vertices;
      dl[0] = 0x12000081;
      p = dl + 1;
      if (verts != NULL) {
        p[0] = (PspAddr(verts) >> 24 & 0xf) << 16 | 0x10000000;
        p[1] = (PspAddr(verts) & 0xffffff) | 0x01000000;
        p += 2;
      }
      p[0] = 0x04040004;
      dl = p + 1;
    } else {
      /* 48-byte vertex block (4 vertices) inlined at the next 16-byte boundary, jumped over */
      buf = (u32 *)(((uintptr_t)(dl + 2) + 0xf) & ~(uintptr_t)0xf);
      jump = buf + 12;
      dl[0] = (PspAddr(jump) >> 24 & 0xf) << 16 | 0x10000000;
      dl[1] = (PspAddr(jump) & 0xffffff) | 0x08000000;
      __builtin_memcpy(buf, sprite->vertices, 4 * sizeof(GfxSpriteVertex));
      jump[0] = 0x12000083;
      p = jump + 1;
      if (buf != NULL) {
        p[0] = (PspAddr(buf) >> 24 & 0xf) << 16 | 0x10000000;
        p[1] = (PspAddr(buf) & 0xffffff) | 0x01000000;
        p += 2;
      }
      p[0] = 0x04040004;
      dl = p + 1;
    }
  }
  GfxPacketEndChunk(packet, dl);
}
