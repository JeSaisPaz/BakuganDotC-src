// bdc 0x089c8870 SndBgmCmdInit
#include "bdc.h"

/* Constructor of a `SndBgmCmd`: runs the `CoreTask` base initialiser (`CoreTaskInit`),
   installs the vtable `0x08af526c` (`+0xc`), clears `kind`, `channel`, `bgmId`, `loop`,
   `fadeSeconds`, `stage`, `done` and `flagB`, creates the global command list with capacity 8 if it
   does not exist (`SndBgmCmdListCreate`) and appends the command to `g_sndBgmCmdList` with
   priority 1000 (`SndBgmCmdListInsert`). Returns `cmd`. */

SndBgmCmd *SndBgmCmdInit(SndBgmCmd *cmd)

{
  CoreTaskInit(&cmd->base);
  (cmd->base).vtable = g_sndBgmCmdVtable;
  cmd->kind = 0;
  cmd->channel = 0;
  cmd->bgmId = 0;
  cmd->loop = '\0';
  cmd->fadeSeconds = 0.0f;
  cmd->stage = 0;
  cmd->done = '\0';
  cmd->flagB = '\0';
  if (g_sndBgmCmdList == (CoreList *)0x0) {
    SndBgmCmdListCreate(8);
  }
  SndBgmCmdListInsert(g_sndBgmCmdList,cmd,1000);
  return cmd;
}

