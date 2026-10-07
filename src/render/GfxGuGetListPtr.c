// bdc 0x08a1edec GfxGuGetListPtr
#include "bdc.h"

/* Returns the current context's list write pointer (`context+8`) and, when `remaining` is non-NULL,
   the number of bytes left in the list (`size - (current - start)`). Used to write raw GE commands
   directly after libgu state. */

u32 *GfxGuGetListPtr(s32 *remaining)

{
  if (remaining != (s32 *)0x0) {
    *remaining = g_guCurrentContext->listSize -
                 (g_guCurrentContext->listCurrent - g_guCurrentContext->listStart);
  }
  return (u32 *)g_guCurrentContext->listCurrent;
}
