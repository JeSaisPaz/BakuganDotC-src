// bdc 0x088f0094 GameEvent470IsSetupOnlyEntry
#include "bdc.h"

/* True when script entry `entry` has fewer than 6 commands and all of them are position/angle setup
   or end opcodes (0x9f..0xa2, 0x92), i.e. it contains no real event. Used by the field input
   handler. */

s32 GameEvent470IsSetupOnlyEntry(GameEvent470 *self, u16 entry)

{
  GameEventScriptEntry *e = &self->base.entries[entry];
  u16 count = e->count;
  GameEventCommand *cmd = &self->base.commands[e->first];
  u16 i = 0;

  if (count < 6) {
    while (cmd->op == 0x9f || cmd->op == 0xa0 || cmd->op == 0xa1 || cmd->op == 0xa2 ||
           cmd->op == 0x92) {
      i++;
      cmd++;
      if (count <= i) {
        return 1;
      }
    }
  }
  return 0;
}
