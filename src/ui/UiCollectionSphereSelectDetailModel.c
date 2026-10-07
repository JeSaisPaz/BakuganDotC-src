// bdc 0x0897c71c UiCollectionSphereSelectDetailModel
#include "bdc.h"

/* Picks the model slot shown in the detail view of `UiCollectionSphere`
   (`+0x1318`: the cursor cell, or slot 6 on category-2 model pages) and marks it active
   (`+0x12c0[slot]`). */

void UiCollectionSphereSelectDetailModel(UiCollectionSphere *self)

{
  u8 kind;
  s8 cell;

  kind = UiCollectionSphereGetPageKind(self, self->page);
  if (kind == 0xff || kind == 0) {
    cell = self->cursor;
  } else {
    cell = 6;
  }
  self->detailCell = cell;
  self->cellActive[self->detailCell] = 1;
}
