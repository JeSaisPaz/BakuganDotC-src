// bdc 0x0892cde0 UiBakuganSelectPhaseClose
#include "bdc.h"

/* Last phase (3) of `UiBakuganSelect`: requests its own close
   (`closeRequested`). */

void UiBakuganSelectPhaseClose(UiBakuganSelect *self)

{
  (self->base).closeRequested = '\x01';
  return;
}

