// bdc 0x089ad2f0 UiPauseSettingsFlashArrow
#include "bdc.h"

/* Flashes the arrow pressed on the current row (`data[1 + item*3 + dir*12]`, `dir` = `+0xb7a`)
   in green/blue over 4 frames (`UiFlashStartRgb`). */

void UiPauseSettingsFlashArrow(UiPauseSettings *self)

{
  UiFlashStartRgb(4.0f, ((GfxSprite **)(self->base).data)[self->cursor * 3 + self->dir * 12 + 1],
                  1, 0, 0, 1, 1);
  return;
}
