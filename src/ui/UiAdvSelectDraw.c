// bdc 0x08917c04 UiAdvSelectDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the adventure partner-select screen (task id 376): when the screen has a
   sprite layer, draws the screen background (`UiScreenDrawBg`) and the sprite layer with mask 1 in
   a render packet at depth 200. Then, for the pedestal model (depth 60) and the partner model
   (depth 80) when present, opens a display-list chunk, writes the lighting state for `camera`
   (`GfxDlWriteLightState`, shadowed), then fog commands derived from the alpha of
   `g_colorWhite` (clamped to 1; at or below 0.0001: black fog with end/scale 10000, else end
   `1 + (1 - a)*800 + 20000`, scale its reciprocal, colour packed from the RGB of `g_colorWhite`
   clamped to [0, 1] and scaled by 255, the bank constant S701), calls the model's draw method
   (vtable slot 8, `fn(model, &list)`) and closes the chunk. */

/* RGB of `g_colorWhite` clamped to [0, 1], scaled by 255 (bank constant S701) and packed as bytes
   (red low); the alpha lane is packed too but masked off by the caller. */
static u32 UiAdvSelectPackWhite(void)
{
  return (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.x) * 255.0f, 23)) |
         (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.y) * 255.0f, 23)) << 8 |
         (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.z) * 255.0f, 23)) << 16 |
         (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.w) * 255.0f, 23)) << 24;
}

static void UiAdvSelectDrawModel(GfxModel *model, GfxCamera *camera, float depth)
{
  union { float f; u32 u; } end;
  union { float f; u32 u; } scale;
  RenderPacket *packet;
  const VtblEntry *draw;
  u32 *list;
  u32 *dl;
  u32 cmd;
  float alpha;
  float dist;

  packet = (RenderPacket *)GfxNewRenderPacket(depth);
  list = GfxPacketBeginChunk(packet);
  dl = GfxDlWriteLightState(list, camera, 1);
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
    cmd = (UiAdvSelectPackWhite() & 0xffffff) | 0xcf000000;
  }
  dl[0] = cmd;
  dl[1] = (end.u >> 8) | 0xcd000000;
  dl[2] = (scale.u >> 8) | 0xce000000;
  list = dl + 3;
  draw = &((const VtblEntry *)model->base.vtable)[8];
  ((void (*)(void *, u32 **))draw->fn)((u8 *)model + draw->delta, &list);
  GfxPacketEndChunk(packet, list);
}

void UiAdvSelectDraw(UiAdvSelect *self)

{
  void *packet;

  if (self->base.spriteLayer != NULL) {
    UiScreenDrawBg(&self->base);
    packet = GfxNewRenderPacket(200.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 1);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
  }
  if (self->pedestal != NULL) {
    UiAdvSelectDrawModel(self->pedestal, (GfxCamera *)self->camera, 60.0f);
  }
  if (self->model != NULL) {
    UiAdvSelectDrawModel((GfxModel *)self->model, (GfxCamera *)self->camera, 80.0f);
  }
}
