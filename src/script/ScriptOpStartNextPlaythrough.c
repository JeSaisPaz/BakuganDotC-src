// bdc 0x08810c9c ScriptOpStartNextPlaythrough
#include "bdc.h"

/* Script opcode (no operands) that starts the next play-through: resets the story event flags
   (`GameEventFlagsAdvanceStage`) and snapshots them into the profile
   (`SaveProfileStoreEventFlags`); sets `eventFlagsStored = 2`, clears `stageStates` (0x80 bytes,
   see `ScriptOpJumpIfStageWordEq`) and `mapMovieWatched` (0x50 bytes); increments the
   play-through counter `playthrough` (clamped to 3); marks the start stages cleared and grants the
   play-through bonus (`SaveProfileUnlockPlaythroughStages`); sets bit 0 (counter 1) or bit 1
   (counter 2) of `playthroughClearBits`; and clears bits 6..0x1e of the script global bitset
   `g_scriptGlobalBits` (`CoreBitsetClear`). Returns 0. */

int ScriptOpStartNextPlaythrough(Script *script)
{
  SaveProfile *profile;
  u8 count;
  u32 bit;

  GameEventFlagsAdvanceStage();
  SaveProfileStoreEventFlags();
  SaveGetProfile()->data->eventFlagsStored = 2;
  profile = SaveGetProfile();
  memset(profile->data->stageStates, 0, 0x80);
  memset(profile->data->mapMovieWatched, 0, 0x50);
  profile = SaveGetProfile();
  profile->data->playthrough = profile->data->playthrough + 1;
  if (profile->data->playthrough >= 4) {
    profile->data->playthrough = 3;
  }
  SaveProfileUnlockPlaythroughStages();
  count = SaveGetProfile()->data->playthrough;
  if (count < 2) {
    if (count > 0) {
      SaveGetProfile()->data->playthroughClearBits |= 1;
    }
  } else if (count < 3) {
    SaveGetProfile()->data->playthroughClearBits |= 2;
  }
  for (bit = 6; bit <= 0x1e; bit++) {
    CoreBitsetClear(bit, g_scriptGlobalBits);
  }
  return 0;
}
