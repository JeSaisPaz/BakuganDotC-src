// bdc 0x0898e648 UiCollectionFigureStartHelpTextFade
#include "bdc.h"

/* Starts the fade of the description text of `UiCollectionFigure`: clears
   the step `+0xeb8` and sets the text alpha `+0xeac` to 0 (fade in) or 1 (`out`); advanced by
   `UiCollectionFigureHelpTextFadeDone`. */

void UiCollectionFigureStartHelpTextFade(UiCollectionFigure *self, u8 out)

{
  if (out == '\0') {
    self->descFade = 0.0f;
    self->helpAlpha = 0.0f;
    return;
  }
  self->descFade = 0.0f;
  self->helpAlpha = 1.0f;
  return;
}

