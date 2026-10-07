// bdc 0x08989f90 UiCollectionTheaterCheckConfirm
#include "bdc.h"

/* Returns 0 unless Cross was pressed in `UiCollectionTheater`; then 1
   when the selected slot holds an unlocked scene and 2 when it is locked. */

int UiCollectionTheaterCheckConfirm(UiScreen *screen)

{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;

  if ((screen->pad->pressed & 0x4000) != 0) {
    if (self->sceneId[self->cursor + self->page * 6] != 0xff) {
      return 1;
    }
    return 2;
  }
  return 0;
}
