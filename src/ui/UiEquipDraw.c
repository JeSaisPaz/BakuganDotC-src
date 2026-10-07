// bdc 0x08957bd8 UiEquipDraw
#include "bdc.h"

/* Draw method (vtable `0x08af4e94` slot +0x24) of the Bakugan/gear loadout screen before a battle
   (task 302, `UiEquipCtor`): when the screen has a sprite layer, draws the shared background
   (`UiScreenDrawBg`) and the sprite layer in five render packets (z 100/400/30/60/500 for layer
   masks 1/2/4/8/0x10). Then, unless more than two players are shown with `g_equipPendingFlag` set,
   one packet (z 40) with the pedestal model of every occupied slot; and unless `g_equipPendingFlag`
   is 2, one packet (z 50) with the Bakugan model of every occupied slot. Each model is drawn with the
   lighting state of its slot camera (`slotCameras[slot]`, shadowed), fog derived from the alpha of
   `g_colorWhite` (clamped to 1; at or below 0.0001: black fog with end/scale 10000, else end
   `1 + (1 - a)*800 + 20000`, scale its reciprocal, colour packed from `g_colorWhite` with the VFPU,
   S701 = 255 bank constant), scissored to the slot rectangle from `UiEquipGetSlotViewport` (reset
   to 480x272 afterwards). Finally draws the name and help text (`UiEquipDrawNameText`,
   `UiEquipDrawHelpText`). */

/* vsat0.q, vscl.q by S701 (255), vf2iz.q 23, vi2uc.q of g_colorWhite */
static u32 UiEquipPackWhite(void)
{
  return (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.x) * 255.0f, 23)) |
         (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.y) * 255.0f, 23)) << 8 |
         (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.z) * 255.0f, 23)) << 16 |
         (u32)VfI2uc(VfF2iz(VfSat0(g_colorWhite.w) * 255.0f, 23)) << 24;
}

static void UiEquipDrawSlotModels(UiEquip *self, GfxModel **models, float depth)
{
  union { float f; u32 u; } end;
  union { float f; u32 u; } scale;
  float rect[4];
  RenderPacket *packet;
  const VtblEntry *draw;
  u32 *list;
  u32 *dl;
  u32 cmd;
  float alpha;
  float dist;
  s32 x;
  s32 y;
  u32 right;
  u32 bottom;
  s32 slot;

  packet = (RenderPacket *)GfxNewRenderPacket(depth);
  list = GfxPacketBeginChunk(packet);
  for (slot = 0; slot < 4; slot++) {
    if (models[slot] == NULL) {
      continue;
    }
    dl = GfxDlWriteLightState(list, &self->slotCameras[slot], 1);
    list = dl;
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
      else {
        scale.f = 0.0f;
      }
      cmd = (UiEquipPackWhite() & 0xffffff) | 0xcf000000;
    }
    dl[0] = cmd;
    dl[1] = (end.u >> 8) | 0xcd000000;
    dl[2] = (scale.u >> 8) | 0xce000000;
    list = dl + 3;

    /* scissor to the slot rectangle (x, y, w, h) */
    UiEquipGetSlotViewport(rect, &self->base, (u8)slot);
    y = (s32)rect[1];
    x = (s32)rect[0];
    bottom = (u32)((s32)rect[3] + y - 1) << 10;
    right = (u32)((s32)rect[2] + x - 1);
    list[0] = ((u32)y << 10) | 0xd4000000 | (u32)x;
    list[1] = bottom | 0xd5000000 | right;
    list[2] = 0x15000000;
    list[3] = bottom | 0x16000000 | right;
    list += 4;

    draw = &((const VtblEntry *)models[slot]->base.vtable)[8];
    ((void (*)(void *, u32 **))draw->fn)((u8 *)models[slot] + draw->delta, &list);

    /* reset the scissor to the full 480x272 screen */
    list[0] = 0xd4000000;
    list[1] = 0xd5043ddf;
    list[2] = 0x15000000;
    list[3] = 0x16043ddf;
    list += 4;
  }
  GfxPacketEndChunk(packet, list);
}

void UiEquipDraw(UiEquip *self)

{
  void *packet;
  int skipPedestals;

  skipPedestals = 0;
  if (self->base.spriteLayer != NULL) {
    UiScreenDrawBg(&self->base);
    packet = GfxNewRenderPacket(100.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 1);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(400.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 2);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(30.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 4);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(60.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 8);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(500.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 0x10);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
  }
  if (self->playerCount >= 3 && g_equipPendingFlag != 0) {
    skipPedestals = 1;
  }
  if (!skipPedestals) {
    UiEquipDrawSlotModels(self, self->pedestalModels, 40.0f);
  }
  if (g_equipPendingFlag != 2) {
    UiEquipDrawSlotModels(self, self->bakuganModels, 50.0f);
  }
  UiEquipDrawNameText(self);
  UiEquipDrawHelpText(self);
}
