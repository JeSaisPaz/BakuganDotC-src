// bdc 0x0898c1d4 UiCollectionFigureSetResultNone
#include "bdc.h"

/* Sets menu result 0 for `UiCollectionFigure` (`UiSetMenuResult`) when
   it exits. */

void UiCollectionFigureSetResultNone(UiCollectionFigure *self)

{
  UiSetMenuResult(&self->base,0);
  return;
}

