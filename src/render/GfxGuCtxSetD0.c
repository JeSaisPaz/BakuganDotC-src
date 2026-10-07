// bdc 0x08a1f0cc GfxGuCtxSetD0
#include "bdc.h"

/* Stores `value` in field `+0xd0` of the current libgu context (`0x08afcf9c`). */

void GfxGuCtxSetD0(u32 value)

{
  g_guCurrentContext->ctxD0 = value;
  return;
}

