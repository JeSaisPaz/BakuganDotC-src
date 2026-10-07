// bdc 0x0896c9c8 UiCardEquipStartPress
#include "bdc.h"

/* Starts the press animation of the confirmed item of `UiCardEquip`: the tab
   button (row 0) or card icon (row 1) via `UiFlashStart`, or the gauge arrow (row 2) via
   `UiFlashStartRgb`, 2 frames. Any other row does nothing. */

void UiCardEquipStartPress(UiCardEquip *self)
{
  GfxSprite **sprites;
  int row = self->row;

  if (row < 1) {
    if (row >= 0) {
      sprites = (GfxSprite **)self->base.data;
      UiFlashStart(2.0f, sprites[self->groups[4][0] + self->rowCursor[row]], 0, 0);
    }
  } else if (row < 2) {
    sprites = (GfxSprite **)self->base.data;
    UiFlashStart(2.0f,
                 sprites[self->groups[8][0] + self->selBakugan * 4 + self->rowCursor[row]], 0, 0);
  } else if (row < 3) {
    sprites = (GfxSprite **)self->base.data;
    UiFlashStartRgb(2.0f,
                    sprites[self->groups[14][0] + self->selBakugan * 6 +
                            (u8)self->gaugeDir * 3 + 2],
                    1, 0, 0, 1, 1);
  }
}
