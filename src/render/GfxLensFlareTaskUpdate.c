// bdc 0x088a1448 GfxLensFlareTaskUpdate
#include "bdc.h"

/* Update of the lens-flare task (id 482). Does nothing unless the task is enabled and a camera
   (`g_gfxActiveCamera`) exists. On the first update it creates one sprite per
   `g_gfxLensFlareElems` entry (blend mode 2) on its layer. While task 0x1e1 does not exist it
   projects the sun position to the screen (`GfxCameraProjectPointFacing`), takes an intensity from
   the vertical distance to the screen centre (240, 136) and the projected depth, and probes the
   frame buffer 10 px around the sun (`GfxReadFrontBufferAlpha`): the sun counts as seen when one
   probe returns alpha 0xb2 and no map flash (`g_btlMapFlashState`) is running. With a positive
   intensity the fade (`unk18`) rises by 0.1 while seen and drops by 0.05 otherwise. The sprites
   are spread along the sun -> centre line, sized and shown with alpha
   intensity * elem alpha * fade * 0.8. With no intensity or no fade, all sprites are hidden. */

void GfxLensFlareTaskUpdate(CoreTask *task)
{
  GfxLensFlareTask *self = (GfxLensFlareTask *)task;
  float screen[4] __attribute__((aligned(16)));
  float center[4] __attribute__((aligned(16)));
  float lerped[2];
  GfxSprite *sprite;
  float intensity;
  float along;
  float fade;
  float size;
  s32 hidden;
  s32 x;
  s32 y;
  s32 i;

  if (self->enabled == 0) {
    return;
  }
  if (g_gfxActiveCamera == NULL) {
    return;
  }
  along = 0.0f;
  intensity = 0.0f;
  if (self->unk14 == 0) {
    for (i = 0; i < 8; i++) {
      sprite = GfxSpriteLayerCreateSpriteByName(self->layer, g_gfxLensFlareElems[i].texName,
                                                (const float *)&g_gfxVecZero, true);
      self->sprites[i] = sprite;
      sprite->blendMode = 2;
    }
    self->unk14 = self->unk14 + 1;
  }
  hidden = 0;
  center[0] = 240.0f;
  center[1] = 136.0f;
  center[2] = 0.0f;
  center[3] = 0.0f;
  if (CoreTaskExists(0x1e1) == 0) {
    GfxCameraProjectPointFacing(g_gfxActiveCamera, screen, self->sunPos);
    intensity = ((400.0f - fabsf(screen[1] - center[1])) + 60.0f) * 0.0025f;
    if (intensity < 0.0f) {
      intensity = 0.0f;
    }
    x = (s32)screen[0];
    y = (s32)screen[1];
    intensity = (-screen[2] - 0.2f) * 1.4f * intensity;
    if (g_btlMapFlashState != 0 ||
        (GfxReadFrontBufferAlpha(x - 10, y) != 0xb2 &&
         GfxReadFrontBufferAlpha(x + 10, y) != 0xb2 &&
         GfxReadFrontBufferAlpha(x, y - 10) != 0xb2 &&
         GfxReadFrontBufferAlpha(x, y + 10) != 0xb2)) {
      hidden = 1;
    }
  }
  if (!(intensity <= 1.0f)) {
    intensity = 1.0f;
  }
  if (intensity <= 0.0f) {
    self->unk18 = 0.0f;
    self->unk1c = 0;
    for (i = 0; i < 8; i++) {
      self->sprites[i]->flags &= ~1u;
    }
    return;
  }
  self->unk1c = self->unk1c + 1;
  if (hidden != 0) {
    fade = self->unk18 - 0.05f;
    self->unk18 = fade;
    if (fade < 0.0f) {
      self->unk18 = 0.0f;
    }
  } else {
    fade = self->unk18 + 0.1f;
    self->unk18 = fade;
    if (!(fade <= 1.0f)) {
      self->unk18 = 1.0f;
    }
  }
  if (self->unk18 <= 0.0f) {
    if (hidden != 0) {
      self->unk1c = 0;
    }
    for (i = 0; i < 8; i++) {
      self->sprites[i]->flags &= ~1u;
    }
    return;
  }
  for (i = 0; i < 8; i++) {
    size = ((float)i * 0.5f + 1.0f) * g_gfxLensFlareElems[i].size;
    along = along + g_gfxLensFlareElems[i].step;
    /* lerped = screen + (center - screen) * along (vsub.q/vscl.q/vadd.q) */
    lerped[0] = screen[0] + (center[0] - screen[0]) * along;
    lerped[1] = screen[1] + (center[1] - screen[1]) * along;
    self->sprites[i]->posX = lerped[0];
    self->sprites[i]->posY = lerped[1];
    UiSpriteSetSize(size, size, self->sprites[i]);
    self->sprites[i]->flags |= 1;
    self->sprites[i]->alpha = intensity * g_gfxLensFlareElems[i].alpha * self->unk18 * 0.8f;
  }
}
