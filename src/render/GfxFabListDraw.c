// bdc 0x089f8a34 GfxFabListDraw
#include "bdc.h"

/* Runs `GfxFabDraw` on every fab object of a linked list (`list->head`, `next` at `+4`). Used by
   battle and UI draw handlers. */

void GfxFabListDraw(void **list)

{
  GfxFab *fab;
  
  for (fab = *list; fab != (GfxFab *)0x0; fab = fab->next) {
    GfxFabDraw(fab);
  }
  return;
}

