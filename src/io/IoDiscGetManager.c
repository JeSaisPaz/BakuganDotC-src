// bdc 0x089f9438 IoDiscGetManager
#include "bdc.h"

/* Returns the disc-access manager `g_discSimple` (no NULL test; check `IoDiscHasManager`
   first). */

void *IoDiscGetManager(void)

{
  return g_discSimple;
}

