// bdc 0x08a1099c GmoTextureListRemove
#include "bdc.h"

/* Unlinks `item` from a `GmoImage` list of a `GmoTexture` (head pointer `*list`, linked through
   `GmoImage.next`): walks the list, splices `item` out and clears its `next`; nothing when `list`
   or `item` is NULL or `item` is not in the list. The predecessor inherits `item`'s reference on
   its successor; the list's reference on `item` passes to the caller. */

void GmoTextureListRemove(GmoImage **list, GmoImage *item)

{
  GmoImage *cur;
  
  if ((list != (GmoImage **)0x0) && (item != (GmoImage *)0x0)) {
    for (cur = *list; cur != (GmoImage *)0x0; cur = cur->next) {
      if (cur == item) {
        *list = item->next;
        item->next = (GmoImage *)0x0;
        return;
      }
      list = &cur->next;
    }
  }
  return;
}

