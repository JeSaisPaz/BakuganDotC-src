// bdc 0x0898d9a4 UiCollectionFigureSetModelSpin
#include "bdc.h"

/* Clears the 12-byte spin state at `+0x1264` of `UiCollectionFigure` and
   sets its enable byte to `enable`; the spin runs in `UiCollectionFigureSpinSelectedModel`. */

void UiCollectionFigureSetModelSpin(UiCollectionFigure *self, u8 enable)

{
  memset(&self->spinOn,0,0xc);
  self->spinOn = enable;
  return;
}

