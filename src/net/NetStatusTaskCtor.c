// bdc 0x08943b00 NetStatusTaskCtor
#include "bdc.h"

/* Constructor of the netplay status overlay task (task id 2002 = 0x7d2, 0x30 bytes, vtable
   `0x08af4c9c`; removed by `NetPlayShutdown`): clears the state, the show flag `0x08ac1bd0` and
   creates a `UiTextBoxCtor` text box at `+0x20` (depth 25000.0). */

NetStatusTask *NetStatusTaskCtor(NetStatusTask *self)

{
  bool fromLow;
  UiTextBox *box;
  UiTextBox *newBox;
  
  CoreTaskInit(&self->base);
  (self->base).vtable = &g_netStatusTaskVtbl;
  self->state = 0;
  self->step = 0;
  self->frame = 0;
  self->alpha = 0.0;
  g_netStatusShow = '\0';
  self->glyphs = (GfxSprite *)0x0;
  self->glyphCount = 0;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  box = MemAlloc(0x10,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  newBox = (UiTextBox *)0x0;
  if (box != (UiTextBox *)0x0) {
    UiTextBoxCtor(box);
    newBox = box;
  }
  self->box = newBox;
  newBox->packetDepth = 25000.0;
  self->message = 0;
  return self;
}

