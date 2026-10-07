// bdc 0x08a1f0dc GfxGuCtxSetD4
#include "bdc.h"

/* Stores `value` in field `+0xd4` of the current libgu context (`0x08afcf9c`). */

void GfxGuCtxSetD4(u32 value)

{
  g_guCurrentContext->ctxD4 = value;
  return;
}

