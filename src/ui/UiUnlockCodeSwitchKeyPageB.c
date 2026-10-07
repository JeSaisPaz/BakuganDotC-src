// bdc 0x08992d44 UiUnlockCodeSwitchKeyPageB
#include "bdc.h"

/* Function key 1 of the `UiUnlockCode` keyboard (row 4): leaves the extra page
   (`+0x89` → 0) or swaps the character page `+0x84` 0 ↔ 0x28 and 0x50 ↔ 0x78, then redraws
   the keyboard (`UiUnlockCodeLayoutKeyboard`) and plays sound 6. */

void UiUnlockCodeSwitchKeyPageB(UiUnlockCode *self)

{
  SndManager *mgr;
  int off;
  
  if (self->page == '\x01') {
    self->page = '\0';
  }
  else {
    off = self->pageOffset;
    if (off != 0xa0) {
      if (off == 0x78) {
        self->pageOffset = 0x50;
      }
      else if (off == 0x50) {
        self->pageOffset = 0x78;
      }
      else if (off == 0x28) {
        self->pageOffset = 0;
      }
      else if (off == 0) {
        self->pageOffset = 0x28;
      }
    }
  }
  UiUnlockCodeLayoutKeyboard(self);

  if (SndHasManager()) {
    mgr = SndGetManager();
    SndManagerPlay(mgr,6,0,0);
  }
  return;
}

