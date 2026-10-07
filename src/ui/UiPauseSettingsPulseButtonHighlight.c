// bdc 0x089ad6a0 UiPauseSettingsPulseButtonHighlight
#include "bdc.h"

/* On the button row pulses the highlight sprite (`data+0xe0`) colour-add between 0 and 0.5 (state
   `+0x93f`, `+0x958`). */

void UiPauseSettingsPulseButtonHighlight(UiPauseSettings *self)
{
  float add;
  GfxSprite *sprite;

  if (self->cursor >= 4) {
    if (self->highlightFalling == 0) {
      add = self->highlightAdd + 0.0125f;
      self->highlightAdd = add;
      if (!(add < 0.5f)) {
        add = 0.5f;
        self->highlightAdd = 0.5f;
        self->highlightFalling = 1;
      }
    } else {
      add = self->highlightAdd - 0.0125f;
      self->highlightAdd = add;
      if (add <= 0.0f) {
        self->highlightAdd = 0.0f;
        add = 0.0f;
        self->highlightFalling = 0;
      }
    }
    add = 0.5f - add;
    sprite = ((GfxSprite **)(self->base).data)[0xe0 / 4];
    sprite->addColor[0] = add;
    sprite->addColor[1] = add;
    sprite->addColor[2] = add;
    sprite->addColor[3] = 1.0f;
  }
}
