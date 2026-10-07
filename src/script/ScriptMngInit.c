// bdc 0x089cb8dc ScriptMngInit
#include "bdc.h"

/* Constructor of the script manager: resets the script package pointer to the built-in default
   table (`ScriptPackageInitDefault`), allocates and clears the script variable block
   (`ScriptVarsInit`), empties the running-script list `g_scriptList` and starts the boot script
   by spawning `"program/boot.lzs"` (`ScriptSpawn`). Returns `mng`. */

void *ScriptMngInit(void *mng)

{
  ScriptPackageInitDefault();
  ScriptVarsInit();
  g_scriptList = (Script *)0x0;
  ScriptSpawn("program/boot.lzs");
  return mng;
}

