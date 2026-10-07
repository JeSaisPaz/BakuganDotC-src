// bdc 0x08979ffc UiCollectionSphereGetPageKind
#include "bdc.h"

/* Returns the kind of page `page` in `UiCollectionSphere`: `page % 3` in
   category 2 (0 = special-item page with one cell, 1/2 = model pages), else 0xff. */

u8 UiCollectionSphereGetPageKind(UiCollectionSphere *self, u8 page)

{
  if (self->category == '\x02') {
    return page % 3;
  }
  return 0xff;
}

