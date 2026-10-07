// bdc 0x0898b740 UiCollectionFigureDraw
#include "bdc.h"

/* Draw method (vtable `0x08af501c` slot +0x24) of the figure collection screen (task 314,
   `maybe_UiScreen314Ctor`; 3x2 grid pages of collected metal figures shown as 3D models).
   When the sprite layer exists: the background (`UiScreenDrawBg`) and sprite layer masks
   1/2/4/8 in render packets z 50/200/300/400. Then each active dim fade (`dim[0]` at z 250,
   `dim[1]` at z 350) as a full-screen black rectangle of alpha `value` (2D state, screen camera
   `g_gfxScreenCamera`, blend preset 1). Then a packet at z 48 holding every present model whose
   `slowSpin` is 0 (light state of its camera, fog from the alpha of `g_colorWhite`, the model's
   draw method), followed by a depth-clear pass of 32 through-mode sprite vertices over the
   screen; then a packet at z 390 holding every model whose `slowSpin` is 1; finally the help text
   (`UiCollectionFigureDrawHelp`).
   The colour pack is lifted from VFPU (S701 = 255.0 bank constant). */

/* Fog commands for the current `g_colorWhite` alpha: clamped to <= 1; at or below 0.0001
   FOG1 = FOG2 = 10000 and colour 0, else far = 1 + (1 - a) * 800, FOG1 = far + 20000,
   FOG2 = 1 / far (0 when far == 0), colour = packed RGB of `g_colorWhite`. */
static u32 *UiCollectionFigureWriteFog(u32 *dl)
{
  union { float f; u32 u; } fog1, fog2;
  float alpha;
  float far;
  u32 color;
  u32 packed;

  alpha = g_colorWhite.w;
  if (!(alpha <= 1.0f)) {
    alpha = 1.0f;
  }
  if (alpha <= 0.0001f) {
    fog1.f = 10000.0f;
    color = 0xcf000000;
    fog2.f = 10000.0f;
  } else {
    far = 1.0f + (1.0f - alpha) * 800.0f;
    fog1.f = far + 20000.0f;
    if (!(far <= 0.0f)) {
      fog2.f = 1.0f / far;
    } else if (far < 0.0f) {
      fog2.f = 1.0f / far;
    } else {
      fog2.f = 0.0f;
    }
    /* vsat0.q, vscl.q by S701 (255), vf2iz.q 23, vi2uc.q; the alpha byte is masked off */
    packed = (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.x) * 255.0f, 23)) |
             (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.y) * 255.0f, 23)) << 8 |
             (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.z) * 255.0f, 23)) << 16;
    color = (packed & 0xffffff) | 0xcf000000; /* FOGCOLOR */
  }
  dl[0] = color;
  dl[1] = (fog1.u >> 8) | 0xcd000000; /* FOG1 */
  dl[2] = (fog2.u >> 8) | 0xce000000; /* FOG2 */
  return dl + 3;
}

void UiCollectionFigureDraw(UiCollectionFigure *self)
{
  RenderPacket *packet;
  u32 *list;
  u32 *dl;
  u32 *after;
  GfxGeColorVertex16 *verts;
  GfxGeColorVertex16 *v;
  GfxModel *model;
  const VtblEntry *draw;
  ScePspFVector4 colour __attribute__((aligned(16)));
  float rect[4];
  s32 i;

  if (self->base.spriteLayer != NULL) {
    UiScreenDrawBg(&self->base);
    packet = GfxNewRenderPacket(50.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 1);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(200.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 2);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(300.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 4);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(400.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 8);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
  }

  for (i = 0; i < 2; i++) {
    if (self->dim[i].active == 0) {
      continue;
    }
    rect[0] = 0.0f;
    rect[1] = 0.0f;
    rect[2] = 480.0f;
    rect[3] = 272.0f;
    if (i == 0) {
      packet = GfxNewRenderPacket(250.0f);
    } else {
      packet = GfxNewRenderPacket(350.0f);
    }
    dl = GfxPacketBeginChunk(packet);
    dl = GfxDlCall2DState(dl);
    dl = GfxCameraDlWrite(g_gfxScreenCamera, dl, 0xffffffff);
    dl = GfxDlSetBlendState(dl, &g_colorWhite, 0, 1);
    colour.x = 0.0f;
    colour.y = 0.0f;
    colour.z = 0.0f;
    colour.w = self->dim[i].value;
    dl = GfxDlDrawColorRect(packet, dl, rect, &colour);
    GfxPacketEndChunk(packet, dl);
  }

  /* Grid models (slowSpin == 0), then a depth clear. */
  packet = GfxNewRenderPacket(48.0f);
  list = GfxPacketBeginChunk(packet);
  for (i = 0; i < 6; i++) {
    if (self->models[i] == NULL || self->slowSpin[i] != 0) {
      continue;
    }
    dl = GfxDlWriteLightState(list, (GfxCamera *)self->cameras[i], 1);
    list = dl;
    list = UiCollectionFigureWriteFog(dl);
    model = self->models[i];
    draw = &((const VtblEntry *)model->base.vtable)[8];
    ((void (*)(void *, u32 **))draw->fn)((u8 *)model + draw->delta, &list);
  }

  /* Jump over the inline vertex data. */
  dl = list;
  verts = (GfxGeColorVertex16 *)(dl + 2);
  after = (u32 *)((u8 *)verts + 32 * sizeof(GfxGeColorVertex16));
  dl[0] = (((u32)(uintptr_t)after >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
  dl[1] = ((u32)(uintptr_t)after & 0xffffff) | 0x08000000;           /* JUMP */
  v = verts;
  for (i = 0; i < 32; i++) {
    v->x = (s16)((i / 2 + i % 2) * 32);
    v->y = (s16)((i % 2) * 272);
    v->z = 0;
    v++;
  }
  after[0] = 0xd3000401; /* CLEAR on (depth) */
  after[1] = 0x1280011c; /* VTYPE through, 8888, s16 */
  after += 2;
  if (verts != NULL) {
    after[0] = (((u32)(uintptr_t)verts >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
    after[1] = ((u32)(uintptr_t)verts & 0xffffff) | 0x01000000;          /* VADDR */
    after += 2;
  }
  after[0] = 0x04060020; /* PRIM sprites x32 */
  after[1] = 0xd3000000; /* CLEAR off */
  list = after + 2;
  GfxPacketEndChunk(packet, list);

  /* Detail-view models (slowSpin == 1). */
  packet = GfxNewRenderPacket(390.0f);
  list = GfxPacketBeginChunk(packet);
  for (i = 0; i < 6; i++) {
    if (self->models[i] == NULL || self->slowSpin[i] != 1) {
      continue;
    }
    dl = GfxDlWriteLightState(list, (GfxCamera *)self->cameras[i], 1);
    list = dl;
    list = UiCollectionFigureWriteFog(dl);
    model = self->models[i];
    draw = &((const VtblEntry *)model->base.vtable)[8];
    ((void (*)(void *, u32 **))draw->fn)((u8 *)model + draw->delta, &list);
  }
  GfxPacketEndChunk(packet, list);
  UiCollectionFigureDrawHelp(self);
}
