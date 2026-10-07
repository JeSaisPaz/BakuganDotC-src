// bdc 0x08976448 UiCollectionMenuRestoreItemBoxMotion
#include "bdc.h"

/* Replays item-box motion 0 (`restore` = 0) or the last motion `+0x753` of
   `UiCollectionMenu` and fast-forwards it by 500 frames to its end pose
   (item-box virtual methods at vtable entries 6 (takes 500.0f) and 7). */

void UiCollectionMenuRestoreItemBoxMotion(UiCollectionMenu *self, u8 restore)

{
  const VtblEntry *entry;

  if (restore == 0) {
    UiCollectionMenuPlayItemBoxMotion(self, 0);
  }
  else {
    UiCollectionMenuPlayItemBoxMotion(self, self->motion);
  }
  entry = &((const VtblEntry *)self->itemBox->base.vtable)[6];
  ((void (*)(void *, float))entry->fn)((u8 *)self->itemBox + entry->delta, 500.0f);
  entry = &((const VtblEntry *)self->itemBox->base.vtable)[7];
  ((void (*)(void *))entry->fn)((u8 *)self->itemBox + entry->delta);
  return;
}
