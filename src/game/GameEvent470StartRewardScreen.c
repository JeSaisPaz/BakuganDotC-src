// bdc 0x088f0128 GameEvent470StartRewardScreen
#include "bdc.h"

/* Body of opcode 0x93: puts the camera under event control (`GameFieldCameraBeginHold`) and
   classifies `id` by the flag ranges 1, 2, 5, 3, 6 (`GameEventFlagInRange`). For a match it
   stores the range-relative value (`GameEventFlagRangeIndex`) in save-profile word 0x14, sets
   the wait type (5 for ranges 1/2/5, 6 for ranges 3/6) and creates task 0x177 = 375, the
   unlock-result (reward) screen `UiUnlockResultCtor` (`CoreTaskCreateDefault`) with the reward
   kind 1, 3, 9, 5 or 6 as its argument. For range 3 (kind 5) also sets `g_rewardMaxusSet` to
   `value / 6` when that is 0 or 1. No match: nothing after the camera hold. */

void GameEvent470StartRewardScreen(GameEvent470 *self, u16 id)
{
  s32 kind;
  s32 value;

  GameFieldCameraBeginHold((GameFieldCamera *)g_gfxActiveCamera);
  kind = 10;
  value = 0;
  if (GameEventFlagInRange(1, id) != 0) {
    kind = 1;
    value = GameEventFlagRangeIndex(1, (s16)id);
    self->base.waitType = 5;
  } else if (GameEventFlagInRange(2, id) != 0) {
    kind = 3;
    value = GameEventFlagRangeIndex(2, (s16)id);
    self->base.waitType = 5;
  } else if (GameEventFlagInRange(5, id) != 0) {
    kind = 9;
    value = GameEventFlagRangeIndex(5, (s16)id);
    self->base.waitType = 5;
  } else if (GameEventFlagInRange(3, id) != 0) {
    kind = 5;
    value = GameEventFlagRangeIndex(3, (s16)id);
    self->base.waitType = 6;
  } else if (GameEventFlagInRange(6, id) != 0) {
    kind = 6;
    value = GameEventFlagRangeIndex(6, (s16)id);
    self->base.waitType = 6;
  }
  if (kind == 10) {
    return;
  }
  if (kind == 5) {
    if (value / 6 == 0) {
      g_rewardMaxusSet = 0;
    } else if (value / 6 == 1) {
      g_rewardMaxusSet = 1;
    }
  }
  SaveProfileSetWord(SaveGetProfile(), 0x14, (u32)value);
  CoreTaskCreateDefault(0x177, (void *)(intptr_t)kind);
}
