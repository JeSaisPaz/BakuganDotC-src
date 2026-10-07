// bdc 0x089a475c UiMainMenuDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the UiMainMenu screen (task id 300): when the screen has a sprite layer,
   draws the screen background (`UiScreenDrawBg`) and the layer twice, mask 1 into a packet at
   depth 50 and mask 2 at depth 200. Then opens a chunk on a packet at depth 20 and, for the base
   model and each present item model, writes the light state for the menu camera
   (`GfxDlWriteLightState`), the GE fog commands (FOGCOLOR 0xcf, FOG1 0xcd, FOG2 0xce) derived
   from `g_colorWhite` (`w` clamped to 1; `w <= 0.0001` gives end = dist = 10000 and a black fog
   colour, otherwise `d = (1 - w) * 800 + 1`, end `d + 20000`, dist `1 / d` or 0 when `d` is 0, colour
   the saturated rgb scaled to bytes) and calls the model's draw method. Closes the chunk and draws
   the help line (`UiHelpLineDraw`).
   The fog colour byte scale is the VFPU bank constant S701 (255). */

void UiMainMenuDraw(UiMainMenu *self)

{
  RenderPacket *packet;
  const GfxModelVtable *vt;
  u32 *list;
  u32 fogColor;
  float alpha;
  float d;
  union { float f; u32 u; } fogEnd, fogDist;
  int i;

  if (self->base.spriteLayer != NULL) {
    UiScreenDrawBg(&self->base);
    packet = (RenderPacket *)GfxNewRenderPacket(50.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 1);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = (RenderPacket *)GfxNewRenderPacket(200.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 2);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
  }
  packet = (RenderPacket *)GfxNewRenderPacket(20.0f);
  list = GfxPacketBeginChunk(packet);

  if (self->baseModel != NULL) {
    list = GfxDlWriteLightState(list, (GfxCamera *)self->camera, 1);
    alpha = g_colorWhite.w;
    if (!(alpha <= 1.0f)) {
      alpha = 1.0f;
    }
    if (alpha <= 0.0001f) {
      fogColor = 0xcf000000;
      fogEnd.f = 10000.0f;
      fogDist.f = 10000.0f;
    }
    else {
      d = (1.0f - alpha) * 800.0f + 1.0f;
      fogEnd.f = d + 20000.0f;
      if (!(d <= 0.0f)) {
        fogDist.f = 1.0f / d;
      }
      else if (d < 0.0f) {
        fogDist.f = 1.0f / d;
      }
      else {
        fogDist.f = 0.0f;
      }
      /* vsat0.q, vscl.q by S701 (255), vf2iz.q 23, vi2uc.q; alpha byte masked off */
      fogColor = (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.x) * 255.0f, 23)) |
                 (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.y) * 255.0f, 23)) << 8 |
                 (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.z) * 255.0f, 23)) << 16 |
                 0xcf000000;
    }
    list[0] = fogColor;
    list[1] = (fogEnd.u >> 8) | 0xcd000000;
    list[2] = (fogDist.u >> 8) | 0xce000000;
    list = list + 3;
    vt = (const GfxModelVtable *)((GfxModel *)self->baseModel)->base.vtable;
    vt->draw((u8 *)self->baseModel + vt->drawAdjust, &list);
  }

  for (i = 0; i < 5; i++) {
    if (self->models[i] != NULL) {
      list = GfxDlWriteLightState(list, (GfxCamera *)self->camera, 1);
      alpha = g_colorWhite.w;
      if (!(alpha <= 1.0f)) {
        alpha = 1.0f;
      }
      if (alpha <= 0.0001f) {
        fogColor = 0xcf000000;
        fogEnd.f = 10000.0f;
        fogDist.f = 10000.0f;
      }
      else {
        d = (1.0f - alpha) * 800.0f + 1.0f;
        fogEnd.f = d + 20000.0f;
        if (!(d <= 0.0f)) {
          fogDist.f = 1.0f / d;
        }
        else if (d < 0.0f) {
          fogDist.f = 1.0f / d;
        }
        else {
          fogDist.f = 0.0f;
        }
        /* vsat0.q, vscl.q by S701 (255), vf2iz.q 23, vi2uc.q; alpha byte masked off */
        fogColor = (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.x) * 255.0f, 23)) |
                   (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.y) * 255.0f, 23)) << 8 |
                   (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.z) * 255.0f, 23)) << 16 |
                   0xcf000000;
      }
      list[0] = fogColor;
      list[1] = (fogEnd.u >> 8) | 0xcd000000;
      list[2] = (fogDist.u >> 8) | 0xce000000;
      list = list + 3;
      vt = (const GfxModelVtable *)((GfxModel *)self->models[i])->base.vtable;
      vt->draw((u8 *)self->models[i] + vt->drawAdjust, &list);
    }
  }
  GfxPacketEndChunk(packet, list);
  UiHelpLineDraw();
}
