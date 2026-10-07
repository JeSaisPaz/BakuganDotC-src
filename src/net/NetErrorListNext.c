// bdc 0x08a31804 NetErrorListNext
#include "bdc.h"

/* Cursor walk of the net-error list: returns the payload of the next non-removed node from the
   cursor (`+0x8`) onward and advances the cursor past it, or NULL at the end. Same code shape as
   `CorePrioListNext`. */

void *NetErrorListNext(CorePrioList *list)

{
  void *data;
  CorePrioNode *cur;

  cur = list->cursor;
  data = (void *)0x0;
  while (cur != (CorePrioNode *)0x0) {
    bool removed = NetErrorNodeIsRemoved(cur);
    cur = list->cursor;
    if (!removed) {
      data = NetErrorNodeGetData(cur);
      cur = list->cursor;
    }
    cur = NetErrorNodeGetNext(cur);
    list->cursor = cur;
    if (data != (void *)0x0) {
      break;
    }
    cur = list->cursor;
  }
  return data;
}
