// bdc 0x08a2ce0c GameQuestCamDirSwitchEnter
#include "bdc.h"

/* Entry 1 (enter, `+0xc`) of the quest camera mode's sub-state 2 vtable `0x08af6f90`: stores the
   argument `*arg` as the next sub-state index `state+8`, used by `GameQuestCamDirSwitchUpdate`. */

void GameQuestCamDirSwitchEnter(void *state, const s32 *arg)

{
  s32 *fields = (s32 *)state;

  fields[2] = *arg;
}
