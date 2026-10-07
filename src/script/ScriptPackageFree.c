// bdc 0x089c9654 ScriptPackageFree
#include "bdc.h"

/* Frees the loaded script package image (under `MemLock`) and sets `g_scriptPackage` to NULL,
   unless it is NULL or still the built-in default table. */

void ScriptPackageFree(void)

{
  if ((g_scriptPackage != &g_scriptDefaultPackage) && (g_scriptPackage != (void *)0x0)) {
    MemLock();
    MemFree(g_scriptPackage,(char *)0x0,0);
    MemUnlock();
    g_scriptPackage = (void *)0x0;
  }
  return;
}

