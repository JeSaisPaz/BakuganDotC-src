// bdc 0x088603f8 BtlBakuganClearLink
#include "bdc.h"

/* Clears a Bakugan's link to another object: zeroes `+0x15c` (the linked Bakugan's id) and
   `+0x160`, and, when `+0x168` points to an object, zeroes that object's field `+0x18`. */

void BtlBakuganClearLink(BtlBakugan *bakugan)

{
  bakugan->targetId = 0;
  bakugan->targetAux = 0;
  if (bakugan->input != NULL) {
    bakugan->input->holdFrames = 0;
  }
}

