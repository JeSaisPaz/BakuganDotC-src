// bdc 0x0898e430 UiCollectionFigureMarkDetailCell
#include "bdc.h"

/* Records the opened cell of `UiCollectionFigure` (`+0x1270` = cursor) and
   sets its slow-spin flag `+0x1220[cell]` (halves the turn speed in
   `UiCollectionFigureSpinSelectedModel`). */

void UiCollectionFigureMarkDetailCell(UiCollectionFigure *self)

{
  self->detailCell = self->cursor;
  self->slowSpin[self->detailCell] = '\x01';
  return;
}

