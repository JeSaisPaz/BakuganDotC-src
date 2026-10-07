// bdc 0x0898d87c UiCollectionFigureSetModelPulse
#include "bdc.h"

/* Clears the 12-byte cell-model pulse state at `+0x1258` of
   `UiCollectionFigure` and sets its enable byte to `enable`; the pulse
   itself runs in `UiCollectionFigurePulseCellModels`. */

void UiCollectionFigureSetModelPulse(UiCollectionFigure *self, u8 enable)

{
  memset(&self->tintOn,0,0xc);
  self->tintOn = enable;
  return;
}

