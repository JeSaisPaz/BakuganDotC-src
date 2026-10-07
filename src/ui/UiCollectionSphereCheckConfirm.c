// bdc 0x0897c53c UiCollectionSphereCheckConfirm
#include "bdc.h"

/* Returns 0 unless Cross was pressed in `UiCollectionSphere`; then 1 when
   the selected cell holds an entry (`entryIds[page * 6 + cursor]` in categories 0/1; on category-2
   pages by kind (`UiCollectionSphereGetPageKind`): `entryIds[(page / 3) * 6 + cursor]` for kind
   0 or 0xff, `kind1Ids[page / 3]` for kind 1, `kind2Ids[page / 3]` for kind 2) and 2 when it is
   empty; other kinds return 0. */

int UiCollectionSphereCheckConfirm(UiCollectionSphere *self)
{
  u8 kind;

  if ((self->base.pad->pressed & 0x4000) == 0) {
    return 0;
  }
  if (self->category >= 0 && self->category < 2) {
    return self->entryIds[self->cursor + self->page * 6] != 0 ? 1 : 2;
  }
  kind = UiCollectionSphereGetPageKind(self, (u8)self->page);
  if (kind == 0xff || kind == 0) {
    return self->entryIds[self->cursor + (self->page / 3) * 6] != 0 ? 1 : 2;
  }
  if (kind == 1) {
    return self->kind1Ids[self->page / 3] != 0 ? 1 : 2;
  }
  if (kind == 2) {
    return self->kind2Ids[self->page / 3] != 0 ? 1 : 2;
  }
  return 0;
}
