// bdc 0x089320e0 UiGauntletSetupDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the gauntlet card setup screen (task id 373). When the sprite layer
   exists: the background (`UiScreenDrawBg`) and sprite layer masks 1/2/4 in render packets
   z 50/200/300. Then, each in its own packet: the pedestal model (z 60, camera `views[0]`) and
   the Bakugan model (z 80, camera `views[0]`), each followed by a depth-clear pass of 32
   through-mode sprite vertices; the player model (z 90, camera `views[1]`) without a clear.
   Every model gets the light state of its camera and fog from the alpha of `g_colorWhite`
   before its draw method. Finally the name and help text boxes
   (`UiGauntletSetupDrawNameBox`, `UiGauntletSetupDrawHelpBox`).
   The fog colour is `g_colorWhite` clamped to [0, 1], scaled by 255 and packed RGB. */

/* Fog commands for the current `g_colorWhite` alpha: clamped to <= 1; at or below 0.0001
   FOG1 = FOG2 = 10000 and colour 0, else far = 1 + (1 - a) * 800, FOG1 = far + 20000,
   FOG2 = 1 / far (0 when far == 0), colour = packed RGB of `g_colorWhite`. */
static u32 *UiGauntletSetupWriteFog(u32 *dl)
{
  union { float f; u32 u; } fog1, fog2;
  float alpha;
  float far;
  u32 color;
  u32 packed;

  fog2.f = 0.0f;
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
    }
    packed = (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.x) * 255.0f, 23)) |
             (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.y) * 255.0f, 23)) << 8 |
             (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.z) * 255.0f, 23)) << 16 |
             (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.w) * 255.0f, 23)) << 24;
    color = (packed & 0xffffff) | 0xcf000000; /* FOGCOLOR */
  }
  dl[0] = color;
  dl[1] = (fog1.u >> 8) | 0xcd000000; /* FOG1 */
  dl[2] = (fog2.u >> 8) | 0xce000000; /* FOG2 */
  return dl + 3;
}

/* Light state of `camera`, fog, then the model's draw method (vtable entry 8) on `*list`. */
static void UiGauntletSetupDrawModel(u32 **list, GfxCamera *camera, GfxModel *model)
{
  const VtblEntry *draw;

  *list = GfxDlWriteLightState(*list, camera, 1);
  *list = UiGauntletSetupWriteFog(*list);
  draw = &((const VtblEntry *)model->base.vtable)[8];
  ((void (*)(void *, u32 **))draw->fn)((u8 *)model + draw->delta, list);
}

/* Inline depth clear: jumps over 32 sprite vertices covering the screen, then CLEAR on,
   draws them in through mode and CLEAR off. Returns the list end. */
static u32 *UiGauntletSetupWriteDepthClear(u32 *dl)
{
  GfxGeColorVertex16 *verts;
  GfxGeColorVertex16 *v;
  u32 *after;
  s32 i;

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
  return after + 2;
}

void UiGauntletSetupDraw(UiGauntletSetup *self)
{
  RenderPacket *packet;
  u32 *list;

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
  }

  if (self->pedestalModel != NULL) {
    packet = GfxNewRenderPacket(60.0f);
    list = GfxPacketBeginChunk(packet);
    UiGauntletSetupDrawModel(&list, (GfxCamera *)self->views, (GfxModel *)self->pedestalModel);
    list = UiGauntletSetupWriteDepthClear(list);
    GfxPacketEndChunk(packet, list);
  }

  if (self->bakuganModel != NULL) {
    packet = GfxNewRenderPacket(80.0f);
    list = GfxPacketBeginChunk(packet);
    UiGauntletSetupDrawModel(&list, (GfxCamera *)self->views, (GfxModel *)self->bakuganModel);
    list = UiGauntletSetupWriteDepthClear(list);
    GfxPacketEndChunk(packet, list);
  }

  if (self->playerModel != NULL) {
    packet = GfxNewRenderPacket(90.0f);
    list = GfxPacketBeginChunk(packet);
    UiGauntletSetupDrawModel(&list, (GfxCamera *)self->views + 1, (GfxModel *)self->playerModel);
    GfxPacketEndChunk(packet, list);
  }

  UiGauntletSetupDrawNameBox(self);
  UiGauntletSetupDrawHelpBox(self);
}
