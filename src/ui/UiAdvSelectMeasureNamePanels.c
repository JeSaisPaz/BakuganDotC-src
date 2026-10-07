// bdc 0x08917f5c UiAdvSelectMeasureNamePanels
#include "bdc.h"

/* Records, for the two name panels of the adventure partner-select screen (`UiAdvSelectCtor`,
   task 376; cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id, partner, locked, ?}` slots), the
   offsets between the panel sprites (`+0x70`/`+0x74` and the label sprites `+0x78`/`+0x80`, minus 2
   px vertically) into `+0x8dc..+0x8fb`, used to move the labels with their panels. */

void UiAdvSelectMeasureNamePanels(UiAdvSelect *self)
{
  int i;
  GfxSprite **sprites = (GfxSprite **)(self->base).data;
  float *out = self->namePanelOffset;

  for (i = 0; i < 2; i++) {
    GfxSprite *panel = sprites[i + 28];
    if (i == 0) {
      out[0] = panel->posX - sprites[i + 32]->posX;
    } else {
      out[0] = sprites[i + 32]->posX - panel->posX;
    }
    out[1] = (sprites[i + 28]->posY - sprites[i + 32]->posY) - 2.0f;
    if (i == 0) {
      out[2] = sprites[i + 28]->posX - sprites[i + 30]->posX;
    } else {
      out[2] = sprites[i + 30]->posX - sprites[i + 28]->posX;
    }
    out[3] = (sprites[i + 28]->posY - sprites[i + 30]->posY) - 2.0f;
    out += 4;
  }
}
