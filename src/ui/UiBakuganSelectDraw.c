// bdc 0x0892b830 UiBakuganSelectDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the UiBakuganSelect screen (task id 371): when the screen has a sprite
   layer, draws the screen background (`UiScreenDrawBg`) and the sprite layer with masks 1/2/4 in
   render packets at depths 50/200/300. Then, for the pedestal model (depth 60) and the Bakugan model
   (depth 80) when present, opens a display-list chunk, writes the lighting state for `camera`
   (`GfxDlWriteLightState`, shadowed), then fog commands derived from the alpha of
   `g_colorWhite` (clamped to 1; at or below 0.0001: black fog with end/scale 10000, else end
   `1 + (1 - a)*800 + 20000`, scale its reciprocal, colour packed from `g_colorWhite`
   (each lane saturated to [0, 1], scaled by 255, truncated to a byte)), calls the model's draw method (vtable slot 8, `fn(model, &list)`) and
   closes the chunk. */

static u32 UiBakuganSelectPackWhite(void)
{
  return (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.x) * 255.0f, 23)) |
         (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.y) * 255.0f, 23)) << 8 |
         (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.z) * 255.0f, 23)) << 16 |
         (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.w) * 255.0f, 23)) << 24;
}

static void UiBakuganSelectDrawModel(GfxModel *model, GfxCamera *camera, float depth)
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
    cmd = (UiBakuganSelectPackWhite() & 0xffffff) | 0xcf000000;
  }
  dl[0] = cmd;
  dl[1] = (end.u >> 8) | 0xcd000000;
  dl[2] = (scale.u >> 8) | 0xce000000;
  list = dl + 3;
  draw = &((const VtblEntry *)model->base.vtable)[8];
  ((void (*)(void *, u32 **))draw->fn)((u8 *)model + draw->delta, &list);
  GfxPacketEndChunk(packet, list);
}

void UiBakuganSelectDraw(UiBakuganSelect *self)

{
  void *packet;

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
  if (self->pedestal != NULL) {
    UiBakuganSelectDrawModel((GfxModel *)self->pedestal, (GfxCamera *)self->camera, 60.0f);
  }
  if (self->model != NULL) {
    UiBakuganSelectDrawModel((GfxModel *)self->model, (GfxCamera *)self->camera, 80.0f);
  }
}
