// bdc 0x08a108f0 GmoImageListFind
#include "bdc.h"

/* Walks a `GmoImage` list (an image or palette list of a `GmoTexture`, linked through `next` at
   `+4`) and returns the `n`-th node whose `id` (`+8`, `lhu`) equals `id`, or NULL. */

GmoImage *GmoImageListFind(GmoImage *list, u32 id, int n)

{
  while (list != (GmoImage *)0x0) {
    if (list->id == id) {
      n = n + -1;
      if (n < 0) {
        return list;
      }
    }
    list = list->next;
  }
  return (GmoImage *)0x0;
}
