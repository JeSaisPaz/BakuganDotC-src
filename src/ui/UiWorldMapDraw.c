// bdc 0x089973a4 UiWorldMapDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the UiWorldMap screen (task id 310). When the sprite layer exists: the
   background (`UiScreenDrawBg`) and sprite layer masks 1/2/4 in render packets z 50/200/400.
   When `mapModel` exists: a packet at z 20 with the light state of the globe camera, fog from
   the alpha of `g_colorWhite`, the globe model's draw method, then a depth-clear pass of 32
   through-mode sprite vertices over the screen. When `jetModel` exists: a packet at z 60 with the
   same light/fog state and the jet model's draw method. While `confirmActive`: a full-screen
   black rectangle of alpha `dimAlpha` at z 300 (2D state, screen camera `g_gfxScreenCamera`,
   blend preset 1). Finally `UiWorldMapDrawTextSlots` and `UiHelpLineDraw`.
   The fog colour is packed with the VFPU (255.0 scale is the bank constant S701). */

/* Fog commands for the current `g_colorWhite` alpha (inlined twice in the binary): clamped to
   <= 1; at or below 0.0001 FOG1 = FOG2 = 10000 and colour 0, else far = 1 + (1 - a) * 800,
   FOG1 = far + 20000, FOG2 = 1 / far (0 when far == 0), colour = packed RGB of `g_colorWhite`. */
static u32 *UiWorldMapWriteFog(u32 *dl)
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
    /* VFPU: vsat0 / scale by 255 (bank S701) / vf2iz 23 / vi2uc; alpha lane masked off. */
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

void UiWorldMapDraw(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
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

  if (screen->spriteLayer != NULL) {
    UiScreenDrawBg(screen);
    packet = GfxNewRenderPacket(50.0f);
    GfxSpriteLayerSetLayerMask(screen->spriteLayer, 1);
    GfxSpriteLayerDraw(screen->spriteLayer, packet);
    packet = GfxNewRenderPacket(200.0f);
    GfxSpriteLayerSetLayerMask(screen->spriteLayer, 2);
    GfxSpriteLayerDraw(screen->spriteLayer, packet);
    packet = GfxNewRenderPacket(400.0f);
    GfxSpriteLayerSetLayerMask(screen->spriteLayer, 4);
    GfxSpriteLayerDraw(screen->spriteLayer, packet);
  }

  /* Globe model, then a depth clear. */
  if (map->mapModel != NULL) {
    packet = GfxNewRenderPacket(20.0f);
    list = GfxPacketBeginChunk(packet);
    list = GfxDlWriteLightState(list, &map->camera, 1);
    list = UiWorldMapWriteFog(list);
    model = map->mapModel;
    draw = &((const VtblEntry *)model->base.vtable)[8];
    ((void (*)(void *, u32 **))draw->fn)((u8 *)model + draw->delta, &list);

    /* Jump over the inline vertex data. */
    dl = list;
    verts = (GfxGeColorVertex16 *)(dl + 2);
    after = (u32 *)((u8 *)verts + 32 * sizeof(GfxGeColorVertex16));
    dl[0] = ((PspAddr(after) >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
    dl[1] = (PspAddr(after) & 0xffffff) | 0x08000000;           /* JUMP */
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
      after[0] = ((PspAddr(verts) >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
      after[1] = (PspAddr(verts) & 0xffffff) | 0x01000000;          /* VADDR */
      after += 2;
    }
    after[0] = 0x04060020; /* PRIM sprites x32 */
    after[1] = 0xd3000000; /* CLEAR off */
    list = after + 2;
    GfxPacketEndChunk(packet, list);
  }

  /* Jet model. */
  if (map->jetModel != NULL) {
    packet = GfxNewRenderPacket(60.0f);
    list = GfxPacketBeginChunk(packet);
    list = GfxDlWriteLightState(list, &map->camera, 1);
    list = UiWorldMapWriteFog(list);
    model = map->jetModel;
    draw = &((const VtblEntry *)model->base.vtable)[8];
    ((void (*)(void *, u32 **))draw->fn)((u8 *)model + draw->delta, &list);
    GfxPacketEndChunk(packet, list);
  }

  /* Dim behind the confirm dialog. */
  if (map->confirmActive != 0) {
    rect[0] = 0.0f;
    rect[1] = 0.0f;
    rect[2] = 480.0f;
    rect[3] = 272.0f;
    packet = GfxNewRenderPacket(300.0f);
    dl = GfxPacketBeginChunk(packet);
    dl = GfxDlCall2DState(dl);
    dl = GfxCameraDlWrite(g_gfxScreenCamera, dl, 0xffffffff);
    dl = GfxDlSetBlendState(dl, &g_colorWhite, 0, 1);
    colour.x = 0.0f;
    colour.y = 0.0f;
    colour.z = 0.0f;
    colour.w = map->dimAlpha;
    dl = GfxDlDrawColorRect(packet, dl, rect, &colour);
    GfxPacketEndChunk(packet, dl);
  }

  UiWorldMapDrawTextSlots(screen);
  UiHelpLineDraw();
}
