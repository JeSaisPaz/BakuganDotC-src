// bdc 0x08971b0c UiOptionStartArrowPress
#include "bdc.h"

/* Starts the press animation (`UiFlashStartRgb`, 6 frames) of the arrow on the side just used
   (`+0xbb8`) of the selected row of `UiOption`. */

void UiOptionStartArrowPress(UiOption *self)

{
  GfxSprite *(*sides)[4][3] = (GfxSprite *(*)[4][3])(self->base).data;

  UiFlashStartRgb(6.0f, sides[self->arrowSide][(s8)self->cursor][1], 1, 0, 0, 1, 1);
}
