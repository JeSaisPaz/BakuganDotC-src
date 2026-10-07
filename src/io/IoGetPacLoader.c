// bdc 0x089fd838 IoGetPacLoader
#include "bdc.h"

/* Returns `g_ioPacLoader` (singleton accessor, named by `bdc singleton`). */

void *IoGetPacLoader(void)

{
  return g_ioPacLoader;
}

