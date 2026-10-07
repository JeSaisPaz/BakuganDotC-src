// bdc 0x089f89f8 GfxFabListUpdate
#include "bdc.h"

/* Runs `GfxFabUpdate` on every fab object of a linked list (`list->head`, `next` at `+4`). Used
   by the UI background updaters (`UiScreenUpdateBg`, `UiSharedBgUpdate`,
   `UiTitleMenuUpdate`). */

void GfxFabListUpdate(void **list)

{
  GfxFab *fab;
  
  for (fab = *list; fab != (GfxFab *)0x0; fab = fab->next) {
    GfxFabUpdate(fab);
  }
  return;
}

