// bdc 0x089ad538 UiPauseSettingsScaleSelectedButton
#include "bdc.h"

/* On the button row (cursor >= 4) grows the selected button (sprite `cursor+0x2e`), its label
   (`cursor+0x31`) and the highlight sprite (index 0x38) toward 1.2x: `buttonScale` creeps up by
   0.1 per frame while below 1.0, the drawn scale is 1 + 0.2*buttonScale capped at 1.2; the three
   sprites sit at depths -200, -201 and -202. Does nothing while the cursor is on a slider row. */

void UiPauseSettingsScaleSelectedButton(UiPauseSettings *self)
{
  float bs;
  float scale;
  int cursor;
  int idx;
  GfxSprite *sprite;

  cursor = self->cursor;
  if (cursor < 4) {
    return;
  }
  bs = self->buttonScale;
  if (bs < 1.0f) {
    bs = bs + 0.1f;
    self->buttonScale = bs;
  }
  scale = bs * 0.20000005f + 1.0f;
  if (!(scale <= 1.2f)) {
    scale = 1.2f;
  }
  idx = cursor + 0x2e;
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[idx], scale, scale, 0.0f);
  sprite = ((GfxSprite **)self->base.data)[idx];
  sprite->posZ = -200.0f;
  idx = self->cursor + 0x31;
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[idx], scale, scale, 0.0f);
  sprite = ((GfxSprite **)self->base.data)[idx];
  sprite->posZ = -201.0f;
  if (self->cursor >= 4) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[0x38], scale, scale, 0.0f);
    sprite = ((GfxSprite **)self->base.data)[0x38];
    sprite->posZ = -202.0f;
  }
}
