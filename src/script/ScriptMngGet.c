// bdc 0x089cb8d0 ScriptMngGet
#include "bdc.h"

/* Returns the script manager singleton `g_scriptMng` (NULL before `ScriptMngCreate`). */

void *ScriptMngGet(void)

{
  return g_scriptMng;
}

