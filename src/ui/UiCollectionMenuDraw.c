// bdc 0x08973a80 UiCollectionMenuDraw
#include "bdc.h"

/* Draw method (vtable `0x08af4f3c` slot +0x24) of the collection top menu (task 311,
   `maybe_UiScreen311Ctor`): when the sprite layer exists, the background (`UiScreenDrawBg`)
   and sprite layer masks 1/2 in render packets z 40/45; then a render packet z 20 with a chunk
   that, when the item-box model exists, gets the light state (`GfxDlWriteLightState``(camera,
   1)`), fog from `g_colorWhite` (alpha clamped to <= 1; alpha <= 0.0001 gives FOG1 = FOG2 =
   10000 and colour 0; otherwise far = 1 + (1 - alpha) * 800, FOG1 = far + 20000, FOG2 = 1 / far
   (0 when far == 0), colour = RGB saturated to [0, 1], scaled by 255 and packed) and the model's
   draw (vtable slot 8, `(model, &list)`). */

void UiCollectionMenuDraw(UiCollectionMenu *self)
{
  RenderPacket *packet;
  u32 *dl;
  u32 *list[4];
  float alpha;
  float far;
  union { float f; u32 u; } fog1, fog2;
  u32 color;
  u32 packed;
  const VtblEntry *e;

  if (self->base.spriteLayer != NULL) {
    UiScreenDrawBg(&self->base);
    packet = GfxNewRenderPacket(40.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 1);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(45.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 2);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
  }
  packet = GfxNewRenderPacket(20.0f);
  list[0] = GfxPacketBeginChunk(packet);
  if (self->itemBox != NULL) {
    dl = GfxDlWriteLightState(list[0], self->camera, 1);
    list[0] = dl;
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
      /* vsat0 / vscl by 255 / vf2iz 23 / vi2uc; lane 3 is masked off below. */
      packed = (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.x) * 255.0f, 23)) |
               (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.y) * 255.0f, 23)) << 8 |
               (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.z) * 255.0f, 23)) << 16;
      color = (packed & 0xffffff) | 0xcf000000; /* FOGCOLOR */
    }
    dl[0] = color;
    dl[1] = (fog1.u >> 8) | 0xcd000000; /* FOG1 */
    dl[2] = (fog2.u >> 8) | 0xce000000; /* FOG2 */
    list[0] = dl + 3;
    e = &((const VtblEntry *)self->itemBox->base.vtable)[8];
    ((void (*)(void *, u32 **))e->fn)((u8 *)self->itemBox + e->delta, list);
  }
  GfxPacketEndChunk(packet, list[0]);
}
