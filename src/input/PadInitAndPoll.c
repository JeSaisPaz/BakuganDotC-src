// bdc 0x089ce5c4 PadInitAndPoll
#include "bdc.h"

/* Runs `PadInit`(pad, 0) and then one `BootEndOfFrame`(pad) step, so a freshly constructed pad
   is configured and polled once before the first frame. */

void PadInitAndPoll(PadState *pad)

{
  PadInit(pad,'\0');
  BootEndOfFrame(pad);
  return;
}

