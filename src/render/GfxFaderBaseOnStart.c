// bdc 0x08a32498 GfxFaderBaseOnStart
#include "bdc.h"

/* Empty start hook (slot `+0x1c`) of the base fader (vtable `0x08af574c`), called by
   `GfxFaderStart` after it has reset the timer; the screen fader `0x08af577c` inherits it. */

void GfxFaderBaseOnStart(GfxFader *self)

{
  return;
}

