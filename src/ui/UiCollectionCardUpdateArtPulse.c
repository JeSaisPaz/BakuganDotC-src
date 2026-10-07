// bdc 0x08985d6c UiCollectionCardUpdateArtPulse
#include "bdc.h"

/* Runs the card-art pulse of `UiCollectionCard` while `previewOn`:
   step 0 waits 120 frames (`previewTimer`); step 1 copies the selected card's sprite (data
   slot 13 + cursor) into the pulse sprite (data slot 51, `GfxSpriteCopy`), saves its scaleX in
   `pulseBaseScale` and starts a fade-out tween on tween 51 (`UiTweenBegin`, `fadeOut` 1,
   flags 3 = alpha + scale); step 2 grows it from that scale to 1.4x while it fades over 60
   frames (`UiTweenUpdate`) and advances when the update returns non-zero; step 3 waits 15
   frames and loops to step 1. Other steps do nothing. */

#define ART_PULSE_SLOT (0xcc / 4)

void UiCollectionCardUpdateArtPulse(UiCollectionCard *self)
{
  float startScale;

  if (self->previewOn == 0) {
    return;
  }
  switch (self->previewStep) {
  case 0:
    if ((float)self->previewTimer < 120.0f) {
      self->previewTimer = self->previewTimer + 1;
    } else {
      self->previewTimer = 0;
      self->previewStep = 1;
    }
    break;
  case 1:
    GfxSpriteCopy(((GfxSprite **)self->base.data)[13 + self->cursor],
                  ((GfxSprite **)self->base.data)[ART_PULSE_SLOT]);
    startScale = ((GfxSprite **)self->base.data)[ART_PULSE_SLOT]->scaleX;
    self->pulseBaseScale = startScale;
    UiTweenBegin(startScale, 1, ((GfxSprite **)self->base.data)[ART_PULSE_SLOT],
                 &self->tweens[0x33], 3);
    self->previewStep = self->previewStep + 1;
    break;
  case 2:
    if (UiTweenUpdate(self->pulseBaseScale, self->pulseBaseScale * 1.4f, 60.0f, 1,
                      ((GfxSprite **)self->base.data)[ART_PULSE_SLOT], &self->tweens[0x33], 3)) {
      self->previewStep = self->previewStep + 1;
    }
    break;
  case 3:
    if ((float)self->previewTimer < 15.0f) {
      self->previewTimer = self->previewTimer + 1;
    } else {
      self->previewTimer = 0;
      self->previewStep = 1;
    }
    break;
  }
}
