// bdc 0x08912c6c UiUpgradeDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the Bakugan upgrade screen (task id 490): draws the background animation
   list (`GfxFabListDraw(&bgAnimList)`) when `bgData` is set, the sprite layer with masks 1 and 2 in
   render packets at depths 50 and 70, then, when the Bakugan `model` is loaded, a depth-60 chunk:
   lighting state for `camera` (`GfxDlWriteLightState`, shadowed), fog derived from the alpha of
   `g_colorWhite` (clamped to 1; at or below 0.0001: black fog with end/scale 10000, else end
   `1 + (1 - a)*800 + 20000`, scale its reciprocal, colour packed from `g_colorWhite` (each lane
   saturated to [0, 1], times 255, floored to a byte), the scissor/region set to `scissorX/Y/W/H`, the
   model's draw method (vtable slot 8, `fn(model, &list)`), then full-screen scissor/region and a
   depth-only clear drawn as 32 inline through-mode sprites (32 x 272 px strips; the vertex colours
   are left unwritten). Ends with the overlay helper `UiUpgradeDrawMessage`. */

void UiUpgradeDraw(UiUpgrade *self)

{
  union { float f; u32 u; } end;
  union { float f; u32 u; } scale;
  void *packet;
  RenderPacket *chunkPacket;
  GfxModel *model;
  const VtblEntry *draw;
  GfxGeColorVertex16 *verts;
  GfxGeColorVertex16 *v;
  u32 *list;
  u32 *dl;
  u32 *after;
  u32 cmd;
  u32 endX;
  u32 endY;
  float alpha;
  float dist;
  int i;
  u32 rgb;

  if (self->base.bgData != NULL) {
    GfxFabListDraw(&self->base.bgAnimList);
  }
  if (self->base.spriteLayer != NULL) {
    packet = GfxNewRenderPacket(50.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 1);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(70.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 2);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
  }
  if (self->model != NULL) {
    chunkPacket = (RenderPacket *)GfxNewRenderPacket(60.0f);
    list = GfxPacketBeginChunk(chunkPacket);
    dl = GfxDlWriteLightState(list, (GfxCamera *)self->camera, 1);
    list = dl;
    scale.f = 0.0f;
    alpha = g_colorWhite.w;
    if (!(alpha <= 1.0f)) {
      alpha = 1.0f;
    }
    if (alpha <= 0.0001f) {
      end.f = 10000.0f;
      cmd = 0xcf000000;
      scale.f = end.f;
    }
    else {
      dist = 1.0f + (1.0f - alpha) * 800.0f;
      end.f = dist + 20000.0f;
      if (!(dist <= 0.0f)) {
        scale.f = 1.0f / dist;
      }
      else if (dist < 0.0f) {
        scale.f = 1.0f / dist;
      }
      rgb = (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.x) * 255.0f, 23))
          | (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.y) * 255.0f, 23)) << 8
          | (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.z) * 255.0f, 23)) << 16
          | (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.w) * 255.0f, 23)) << 24;
      cmd = (rgb & 0xffffff) | 0xcf000000;
    }
    dl[0] = cmd;                                          /* FOGCOLOR */
    dl[1] = (end.u >> 8) | 0xcd000000;                    /* FOG1 (end) */
    dl[2] = (scale.u >> 8) | 0xce000000;                  /* FOG2 (scale) */
    dl += 3;
    list = dl;

    endY = (u32)(self->scissorH + self->scissorY - 1);
    endX = (u32)(self->scissorW + self->scissorX - 1);
    dl[0] = ((u32)self->scissorY << 10) | 0xd4000000 | (u32)self->scissorX; /* SCISSOR1 */
    dl[1] = (endY << 10) | 0xd5000000 | endX;                                /* SCISSOR2 */
    dl[2] = 0x15000000;                                                      /* REGION1 */
    dl[3] = (endY << 10) | 0x16000000 | endX;                                /* REGION2 */
    list = dl + 4;

    model = (GfxModel *)self->model;
    draw = &((const VtblEntry *)model->base.vtable)[8];
    ((void (*)(void *, u32 **))draw->fn)((u8 *)model + draw->delta, &list);

    dl = list;
    dl[0] = 0xd4000000;                                   /* SCISSOR1 (0, 0) */
    dl[1] = 0xd5043ddf;                                   /* SCISSOR2 (479, 271) */
    dl[2] = 0x15000000;                                   /* REGION1 */
    dl[3] = 0x16043ddf;                                   /* REGION2 (479, 271) */
    dl += 4;

    /* Jump over the inline vertex data. */
    verts = (GfxGeColorVertex16 *)(dl + 2);
    after = (u32 *)((u8 *)verts + 32 * sizeof(GfxGeColorVertex16));
    list = dl;
    dl[0] = (((u32)(uintptr_t)after >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
    dl[1] = ((u32)(uintptr_t)after & 0xffffff) | 0x08000000;           /* JUMP */

    v = verts;
    for (i = 0; i < 32; i++) {
      v->x = (s16)((i / 2 + i % 2) * 32);
      v->y = (s16)((i % 2) * 272);
      v->z = 0;
      v++;
    }

    after[0] = 0xd3000401;                                /* CLEAR on (depth) */
    after[1] = 0x1280011c;                                /* VTYPE through, 8888, s16 */
    after += 2;
    if (verts != NULL) {
      after[0] = (((u32)(uintptr_t)verts >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
      after[1] = ((u32)(uintptr_t)verts & 0xffffff) | 0x01000000;          /* VADDR */
      after += 2;
    }
    after[0] = 0x04060020;                                /* PRIM sprites x32 */
    after[1] = 0xd3000000;                                /* CLEAR off */
    list = after + 2;
    GfxPacketEndChunk(chunkPacket, list);
  }
  UiUpgradeDrawMessage(self);
}
