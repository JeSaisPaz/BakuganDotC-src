// bdc 0x08943c64 NetStatusTaskSetField
#include "bdc.h"

/* `SetField` override (slot 5) of the netplay status overlay: 0-2 base; 3 sets the state `+0x10`
   (resetting the sub-step `+0x14` when it changes); 4 sets the sub-step. */

void NetStatusTaskSetField(NetStatusTask *self, u32 field, u32 value)

{
  if (field < 3) {
    CoreTaskSetField(&self->base,field,value);
    return;
  }
  if ((int)field < 4) {
    if ((2 < (int)field) && (self->state != value)) {
      self->state = value;
      self->step = 0;
      return;
    }
  }
  else if ((int)field < 5) {
    self->step = value;
  }
  return;
}

