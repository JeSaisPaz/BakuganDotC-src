// bdc 0x088100f0 ScriptOpTalkSetMessageFile
#include "bdc.h"

/* Script opcode handler: reads a u16 `slot` and an inline string (skipped); if the talk task (core
   id `0x6e`) exists calls `UiTalkSetMessageFile``(task, slot, name)`, which builds
   `<name>_eu.bin` as the message file for slot 1. Returns 0. */

int ScriptOpTalkSetMessageFile(Script *script)

{
  s16 slot;
  const char *name;
  
  slot = (s16)ScriptReadU16(script);
  name = (const char *)script->operand;
  ScriptSkipString(script);
  if (UiTalkTaskExists() != 0) {
    UiTalkSetMessageFile(UiGetTalkTask(),slot,name);
  }
  return 0;
}

