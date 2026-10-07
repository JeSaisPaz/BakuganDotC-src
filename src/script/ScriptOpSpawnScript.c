// bdc 0x089ca768 ScriptOpSpawnScript
#include "bdc.h"

/* Starts another script and continues without waiting: the operand is an inline NUL-terminated
   script name (skipped with `ScriptSkipString`), passed to `ScriptSpawn`. Returns 0. */

int ScriptOpSpawnScript(Script *script)

{
  const char *name;

  /* The asm takes the name from v0 of ScriptSkipString, which still holds the operand pointer
     loaded before the skip (the function is typed void); reading it first is equivalent. */
  name = (const char *)script->operand;
  ScriptSkipString(script);
  ScriptMngGet();
  ScriptSpawn(name);
  return 0;
}
