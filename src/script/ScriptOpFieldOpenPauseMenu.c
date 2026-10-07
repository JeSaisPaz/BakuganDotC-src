// bdc 0x0880e93c ScriptOpFieldOpenPauseMenu
#include "bdc.h"

/* Script opcode: reads u16 `mode` and, when the field task (id 500, `GameFieldCtor`) exists
   (`CoreTaskFind`), opens the pause menu with `GameFieldOpenPauseMenu``(field, mode)`. Returns
   0. */

int ScriptOpFieldOpenPauseMenu(Script *script)

{
  u32 mode;
  void *task;
  
  mode = ScriptReadU16(script);
  task = CoreTaskFind(500);
  if (task != (void *)0x0) {
    GameFieldOpenPauseMenu(task,mode);
  }
  return 0;
}

