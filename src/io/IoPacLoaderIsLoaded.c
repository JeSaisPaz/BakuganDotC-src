// bdc 0x089fdc20 IoPacLoaderIsLoaded
#include "bdc.h"

/* Returns whether the `.pac` package loader (`g_ioPacLoader`, 8 bytes: `+0` state, `+4`
   request/pack) is in state 2 (loaded). */

bool IoPacLoaderIsLoaded(void *loader)

{
  return ((IoPacLoader *)loader)->state == 2;
}

