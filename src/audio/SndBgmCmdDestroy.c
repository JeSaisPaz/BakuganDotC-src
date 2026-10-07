// bdc 0x089c88fc SndBgmCmdDestroy
#include "bdc.h"

/* Destructor of a `SndBgmCmd` (vtable `0x08af526c` slot 1, reached through `CoreTask`'s virtual
   destructor): resets the vtable word, removes the command from `g_sndBgmCmdList`
   (`SndBgmCmdListRemove`), runs the `CoreTask` base destructor (`CoreTaskDestroy`, which also
   kills a child task stored at `+8`) and, when bit 0 of `flags` is set, frees the object (`MemFree`
   under `MemLock`). Does nothing for NULL. */

void SndBgmCmdDestroy(SndBgmCmd *cmd, u32 flags)

{
  if (cmd != (SndBgmCmd *)0x0) {
    (cmd->base).vtable = g_sndBgmCmdVtable;
    SndBgmCmdListRemove(g_sndBgmCmdList,cmd);
    CoreTaskDestroy(&cmd->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(cmd,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

