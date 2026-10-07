// bdc 0x08a1ee28 sceGuSetMemory
#include "bdc.h"

/* Sets the current libgu context's display list write pointer to `ptr`. */

void sceGuSetMemory(u32 *ptr)

{
  g_guCurrentContext->listCurrent = (unsigned char *)ptr;
}
