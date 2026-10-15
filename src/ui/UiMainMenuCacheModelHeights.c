// bdc 0x089a71cc UiMainMenuCacheModelHeights
#include "bdc.h"

/* Stores each item model's height (`pos[1]`, `+0x24`, all but item 1) as the base of the bobbing animation
   (`+0x9c4..`, 12-byte records) and resets its phase. */

void UiMainMenuCacheModelHeights(UiMainMenu *self)

{
  int i;

  for (i = 0; i < 5; i++) {
    if (i != 1) {
      GfxModel *model = (GfxModel *)self->models[i];
      self->bob[i][0] = model->pos[1];
      self->bob[i][2] = 0.0f;
      self->bob[i][1] = model->pos[1];
    }
  }
}
