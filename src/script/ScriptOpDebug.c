// bdc 0x089cb2a0 ScriptOpDebug
#include "bdc.h"

/* Debug/diagnostic opcode kept in the release script VM: mode 0 checks the heap with
   `MemCheckFragmentation` and, on a fragmentation warning, prints the script name and the source
   line (looked up in the package's line table by `ScriptGetSourceLine`, format ` , Line=%d\n`)
   through `printf`; mode 1 would set a `host0:../../disc/%s` path through `CoreDebugNop`, which
   is an empty stub in this build. Other modes do nothing. Always returns 0. */

int ScriptOpDebug(Script *script)

{
  s32 mode;
  const char *str;
  u32 line;
  char path[256];
  ScriptTrack *track;

  mode = (s32)ScriptReadU32(script);
  /* ScriptSkipString leaves the string start in v0; read it before the skip. */
  str = (const char *)script->operand;
  ScriptSkipString(script);
  if (mode == 0) {
    if (MemCheckFragmentation()) {
      line = 0;
      printf("%s", script->name);
      /* Look up the line of the instruction after this one, then restore pc. */
      script->curTrack->pc = script->curTrack->pc + script->length;
      if (ScriptGetSourceLine(script, &line) != 0) {
        printf(" , Line=%d\n", line);
      }
      else {
        printf("\n");
      }
      track = script->curTrack;
      track->pc = track->pc - script->length;
    }
  }
  else if (mode == 1) {
    /* The original passes `path` (or NULL when the string is empty) to the stub. */
    if (strlen(str) != 0) {
      sprintf(path, "host0:../../disc/%s", str);
      CoreDebugNop();
    }
    else {
      CoreDebugNop();
    }
  }
  return 0;
}
