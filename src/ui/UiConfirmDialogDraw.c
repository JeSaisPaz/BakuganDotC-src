// bdc 0x0890e5e4 UiConfirmDialogDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the yes/no confirm dialog: submits the sprite layer, prints the message
   `g_confirmDialogMessage` through the text printer `textPrinter` at (240, 128) with a white
   outline whose alpha is `textAlpha`, and from sub-state 3 on pulses the add-colour of the button
   selected by `cursor` (sprites[8 + (cursor != 0)]) and of sprites[14] with
   0.3 - (1 - cos(pulsePhase·π))·0.15, clearing the other button's add-colour; `pulsePhase` then
   advances by 0.04 per frame (0.08 when `g_gfxDisplay` skips frames).
   The cosine is the VFPU vcos of pulsePhase·π·(2/π) quarter turns, i.e. cos(pulsePhase·π). */

typedef void (*ConfirmPrintFn)(float x, float y, float z, void *self, const char *text, s32 a,
                               s32 b, s32 c);
typedef void (*ConfirmDrawFn)(void *self, void *packet);

void UiConfirmDialogDraw(UiConfirmDialog *self)
{
  void *packet;
  UiTextPrinter *printer;
  const VtblEntry *entry;
  void *target;
  GfxSprite *sprite;
  float cosv;
  float dim;

  packet = GfxNewRenderPacket(5100.0f);
  if (self->spriteLayer != NULL) {
    GfxSpriteLayerDraw(self->spriteLayer, packet);
  }
  if (self->textPrinter != NULL) {
    printer = self->textPrinter;
    printer->outlineColor[0] = g_colorWhite.x;
    printer->outlineColor[1] = g_colorWhite.y;
    printer->outlineColor[2] = g_colorWhite.z;
    printer->outlineColor[3] = g_colorWhite.w;
    self->textPrinter->outlineColor[3] = self->textAlpha;
    printer = self->textPrinter;
    entry = &printer->layer.vtbl[2];
    ((ConfirmPrintFn)entry->fn)(240.0f, 128.0f, 0.0f, (u8 *)printer + entry->delta,
                                g_confirmDialogMessage, 1, 0, 1);
    printer = self->textPrinter;
    entry = &printer->layer.vtbl[5];
    target = (u8 *)printer + entry->delta;
    packet = GfxNewRenderPacket(5500.0f);
    ((ConfirmDrawFn)entry->fn)(target, packet);
  }
  if ((s32)self->subState >= 3) {
    cosv = __builtin_cosf(self->pulsePhase * 3.1415927f);
    dim = (1.0f - cosv) * 0.5f * 0.3f;

    sprite = self->sprites[8 + (self->cursor == 0)];
    sprite->addColor[0] = 0.0f;
    sprite->addColor[1] = 0.0f;
    sprite->addColor[2] = 0.0f;
    sprite->addColor[3] = 1.0f;

    sprite = self->sprites[8 + (self->cursor != 0)];
    dim = 0.3f - dim;
    sprite->addColor[0] = dim;
    sprite->addColor[1] = dim;
    sprite->addColor[2] = 0.0f;
    sprite->addColor[3] = 1.0f;

    sprite = self->sprites[14];
    sprite->addColor[0] = dim;
    sprite->addColor[1] = dim;
    sprite->addColor[2] = dim;
    sprite->addColor[3] = 1.0f;

    if (g_gfxDisplay->frameSkip != 0) {
      self->pulsePhase = self->pulsePhase + 0.08f;
    } else {
      self->pulsePhase = self->pulsePhase + 0.04f;
    }
  }
}
