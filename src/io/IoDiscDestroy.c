// bdc 0x089f93e4 IoDiscDestroy
#include "bdc.h"

/* Destroys the disc-access manager singleton `g_discSimple` (`IoDiscSimpleDtor(obj, 3)`, destruct and
   free) and clears it. Byte-identical to `ScriptMngDestroy` apart from the global and the
   destructor; called from the boot thread `BootDiscThread`. */

void IoDiscDestroy(void)

{
  if (g_discSimple != (void *)0x0) {
    IoDiscSimpleDtor(g_discSimple,3);
    g_discSimple = (void *)0x0;
  }
  return;
}

