// bdc 0x08943cfc NetStatusTaskGetField
#include "bdc.h"

/* `GetField` override (slot 6) of the netplay status overlay: 0-2 base, 3 state `+0x10`, 4 sub-step
   `+0x14`. */

u32 NetStatusTaskGetField(NetStatusTask *self, u32 field)

{
  u32 result;

  result = 0;
  if (field < 3) {
    return CoreTaskGetField(&self->base, field);
  }
  if ((int)field < 4) {
    if (2 < (int)field) {
      return self->state;
    }
  }
  else if ((int)field < 5) {
    result = self->step;
  }
  return result;
}
