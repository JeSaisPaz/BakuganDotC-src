// bdc 0x089c95e0 ScriptPackageInitDefault
#include "bdc.h"

/* Points `g_scriptPackage` at the built-in script package embedded in the image at `0x08aa5ec0`,
   which holds all 23 shipped scripts; `ScriptPackageFree` knows not to free it. */

void ScriptPackageInitDefault(void)

{
  g_scriptPackage = &g_scriptDefaultPackage;
  return;
}

