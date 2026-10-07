// bdc 0x08982478 UiCollectionCardInitState
#include "bdc.h"

/* Class init of `UiCollectionCard` called by `UiCollectionCardCtor`:
   clears cursor `+0xbcc`, page `+0xbcd`, target page `+0xbce`, page direction `+0xbcf`, builds the
   card list (`UiCollectionCardBuildCardList`), clears the animation records and creates the help
   printer (`UiCollectionCardCreateHelpPrinter`). */

void UiCollectionCardInitState(UiCollectionCard *self)

{
  self->cursor = '\0';
  self->page = '\0';
  self->targetStep = '\0';
  self->targetPage = '\0';
  self->pageDir = '\0';
  UiCollectionCardBuildCardList(self);
  memset(&self->bobOn,0,0xc);
  memset(self->dim,0,sizeof(self->dim));
  memset(&self->fastScroll,0,4);
  memset(&self->helpPrinter,0,0x224);
  UiCollectionCardCreateHelpPrinter(self);
  memset(&self->previewOn,0,0xc);
  return;
}

