// bdc 0x089fdc3c IoPacLoaderIsIdle
#include "bdc.h"

/* Returns whether the `.pac` package loader (`g_ioPacLoader`, 8 bytes: `+0` state, `+4`
   request/pack) is in state 0 (idle). */

bool IoPacLoaderIsIdle(void *loader)

{
  return ((IoPacLoader *)loader)->state == 0;
}

