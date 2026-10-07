// bdc 0x0898e3f8 UiCollectionFigureSelectFlashDone
#include "bdc.h"

/* Advances UI flash slot 0 (`UiFlashStep(0)`) for `UiCollectionFigure`;
   returns 1 when the flash started by `UiCollectionFigureStartSelectFlash` has finished,
   0 while it is still running. */

int UiCollectionFigureSelectFlashDone(UiCollectionFigure *self)
{
  if (UiFlashStep(0) != 0) {
    return 1;
  }
  return 0;
}
