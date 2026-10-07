// bdc 0x0898e44c UiCollectionFigureUnmarkDetailCell
#include "bdc.h"

/* Clears the slow-spin flag `+0x1220[+0x1270]` of the cell opened in
   `UiCollectionFigure` when its detail view closes. */

void UiCollectionFigureUnmarkDetailCell(UiCollectionFigure *self)

{
  self->slowSpin[self->detailCell] = '\0';
  return;
}

