// bdc 0x089caf48 ScriptOpSpawnScriptFmt2
#include "bdc.h"

/* Like `ScriptOpSpawnScriptFmt1` with two arguments: `sprintf(buf, fmt, a, b)` builds the name
   for `ScriptSpawn`; does not wait. */

int ScriptOpSpawnScriptFmt2(Script *script)

{
  const char *format;
  u32 a;
  u32 b;
  char buf[256];
  
  format = (const char *)script->operand;
  ScriptSkipString(script);
  a = ScriptReadU32(script);
  b = ScriptReadU32(script);
  sprintf(buf,format,a,b);
  ScriptMngGet();
  ScriptSpawn(buf);
  return 0;
}

