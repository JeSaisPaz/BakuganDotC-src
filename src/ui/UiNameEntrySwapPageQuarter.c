// bdc 0x08805e1c UiNameEntrySwapPageQuarter
#include "bdc.h"

/* Second page-switch button of `UiNameEntry`: leaves the symbol page if active,
   otherwise toggles the character page `+0x84` between 0 ↔ 0x28 and 0x50 ↔ 0x78; then redraws
   the grid (`UiNameEntryRedrawKeyGrid`) and plays sound 6. */

void UiNameEntrySwapPageQuarter(UiNameEntry *self)

{
  SndManager *mgr;
  int cur;
  
  if (self->symbolPage == '\x01') {
    self->symbolPage = '\0';
  }
  else {
    cur = self->page;
    if (cur != 0xa0) {
      if (cur == 0x78) {
        self->page = 0x50;
      }
      else if (cur == 0x50) {
        self->page = 0x78;
      }
      else if (cur == 0x28) {
        self->page = 0;
      }
      else if (cur == 0) {
        self->page = 0x28;
      }
    }
  }
  UiNameEntryRedrawKeyGrid(self);
  if (SndHasManager()) {
    mgr = SndGetManager();
    SndManagerPlay(mgr,6,0,0);
  }
  return;
}

