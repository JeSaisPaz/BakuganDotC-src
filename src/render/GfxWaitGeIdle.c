// bdc 0x089cebd0 GfxWaitGeIdle
#include "bdc.h"

/* Waits until the GE has finished drawing everything queued so far: `sceGuSync`(0, 0)
   (`sceGeDrawSync`). */

void GfxWaitGeIdle(void)

{
  sceGuSync(0,0);
  return;
}

