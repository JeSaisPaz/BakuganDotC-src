// bdc 0x089cac6c ScriptOpSpawnScriptFmt1
#include "bdc.h"

/* Starts another script whose name is built with `sprintf(buf, fmt, arg)` from an inline format
   string and one value; the 256-byte buffer is passed to `ScriptSpawn` without waiting. */

int ScriptOpSpawnScriptFmt1(Script *script)

{
  const char *format;
  u32 arg;
  char name[256];

  /* The asm takes the format from v0 of ScriptSkipString, which still holds the operand pointer
     loaded before the skip (the function is typed void); reading it first is equivalent. */
  format = (const char *)script->operand;
  ScriptSkipString(script);
  arg = ScriptReadU32(script);
  sprintf(name, format, arg);
  ScriptMngGet();
  ScriptSpawn(name);
  return 0;
}
