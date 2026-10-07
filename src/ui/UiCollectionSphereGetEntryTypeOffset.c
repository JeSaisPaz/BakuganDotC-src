// bdc 0x0897c7e4 UiCollectionSphereGetEntryTypeOffset
#include "bdc.h"

/* Returns the position offset of the selected entry of `UiCollectionSphere`
   from a stack copy of `g_uiCollectionSphereEntryOffsets`, indexed by the entry id: `entryIds` of
   the cell in categories 0/1, else on category-2 pages `kind1Ids` (page kind 1) or `kind2Ids`
   (page kind 2) of group `page / 3`; 0 on the other pages. Used by the detail moves. */

float UiCollectionSphereGetEntryTypeOffset(UiCollectionSphere *self)

{
  float offsets[33];
  u8 kind;

  memcpy(offsets, g_uiCollectionSphereEntryOffsets, sizeof(offsets));
  if (self->category >= 0 && self->category < 2) {
    return offsets[self->entryIds[self->page * 6 + self->cursor]];
  }
  kind = UiCollectionSphereGetPageKind(self, (u8)self->page);
  if (kind == 0xff || kind == 0) {
    return 0.0f;
  }
  if (kind == 1) {
    return offsets[self->kind1Ids[self->page / 3]];
  }
  if (kind == 2) {
    return offsets[self->kind2Ids[self->page / 3]];
  }
  return 0.0f;
}
