// bdc 0x089ad774 UiPauseSettingsRefreshRowArrows
#include "bdc.h"

/* Recolours the two arrows of row `item`: both white, then the left one grey at value 0 and the
   right one grey at 9 (sliders); for the advisor row both grey when
   `UiPauseSettingsAdviceAvailable` is false, else the one pointing at the current state
   (left when the value is 0, right otherwise). */

static void SetArrowColour(GfxSprite *arrow, float grey)
{
  arrow->tint[0] = grey;
  arrow->tint[1] = grey;
  arrow->tint[2] = grey;
  arrow->alpha = 1.0f;
}

void UiPauseSettingsRefreshRowArrows(UiPauseSettings *self, u8 item)
{
  u8 value;
  int row = item * 3;

  SetArrowColour(((GfxSprite **)self->base.data)[row + 1], 1.0f);
  SetArrowColour(((GfxSprite **)self->base.data)[row + 13], 1.0f);
  value = UiPauseSettingsGetItemValue(self, item);
  if (item < 3) {
    if (value == 0) {
      SetArrowColour(((GfxSprite **)self->base.data)[row + 1], 0.5f);
    }
    if (value == 9) {
      SetArrowColour(((GfxSprite **)self->base.data)[row + 13], 0.5f);
    }
  } else if (UiPauseSettingsAdviceAvailable(self) == 0) {
    SetArrowColour(((GfxSprite **)self->base.data)[row + 1], 0.5f);
    SetArrowColour(((GfxSprite **)self->base.data)[row + 13], 0.5f);
  } else if (value == 0) {
    SetArrowColour(((GfxSprite **)self->base.data)[row + 1], 0.5f);
  } else {
    SetArrowColour(((GfxSprite **)self->base.data)[row + 13], 0.5f);
  }
}
