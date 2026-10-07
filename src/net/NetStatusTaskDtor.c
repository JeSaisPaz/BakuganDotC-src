// bdc 0x08943bdc NetStatusTaskDtor
#include "bdc.h"

/* Destructor of the netplay status overlay (id 2002): deletes its text box (`UiTextBoxDelete`),
   chains to `CoreTaskDestroy`. */

void NetStatusTaskDtor(NetStatusTask *self, u32 flags)

{
  UiTextBox *box;
  
  if (self != (NetStatusTask *)0x0) {
    box = self->box;
    (self->base).vtable = &g_netStatusTaskVtbl;
    if (box != (UiTextBox *)0x0) {
      UiTextBoxDelete(box,3);
      self->box = (UiTextBox *)0x0;
    }
    CoreTaskDestroy(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

