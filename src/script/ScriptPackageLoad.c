// bdc 0x089c95f4 ScriptPackageLoad
#include "bdc.h"

/* Loads a script package file from disc: frees the current package unless it is the built-in
   default, then reads the whole file into a new heap block with `IoLoadFile` (raw path, no size
   output) and stores it in `g_scriptPackage`. */

void ScriptPackageLoad(const char *path)

{
  if (g_scriptPackage != &g_scriptDefaultPackage) {
    ScriptPackageFree();
  }
  g_scriptPackage = IoLoadFile(path,(void *)0x0,(u32 *)0x0,'\x01');
  return;
}

