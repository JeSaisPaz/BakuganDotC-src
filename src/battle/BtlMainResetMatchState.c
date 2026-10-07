// bdc 0x0884e63c BtlMainResetMatchState
#include "bdc.h"

/* Resets the per-battle profile words: remaining time word 2 = word 0x19 (seconds, default 300) ×
   60 or -1 (none, always in game mode 1), goal/scores 0xb..0x11, and unless `keepRounds` the round
   counter 0x1e and round table `roundResults`; in ranked mode creates task 0x69 and sets the goal
   (30 when untimed). */

void BtlMainResetMatchState(BtlMain *self)
{
  u32 seconds;
  s32 rankedMode;
  s32 i;

  seconds = SaveProfileGetWord(SaveGetProfile(), 0x19);
  if (seconds == 0) {
    seconds = 300;
  }
  if (seconds == 0xffffffff) {
    SaveProfileSetWord(SaveGetProfile(), 2, 0xffffffff);
  }
  else {
    SaveProfileSetWord(SaveGetProfile(), 2, seconds * 60);
  }
  SaveProfileSetWord(SaveGetProfile(), 0xb, 0);
  SaveProfileSetWord(SaveGetProfile(), 0xc, 0);
  SaveProfileSetWord(SaveGetProfile(), 0xd, 0);
  SaveProfileSetWord(SaveGetProfile(), 0xe, 0xffffffff);
  SaveProfileSetWord(SaveGetProfile(), 0xf, 0xffffffff);
  SaveProfileSetWord(SaveGetProfile(), 0x10, 0xffffffff);
  SaveProfileSetWord(SaveGetProfile(), 0x11, 0xffffffff);
  if (self->keepRounds == 0) {
    SaveProfileSetWord(SaveGetProfile(), 0x1e, 0);
    for (i = 0; i < 5; i++) {
      self->roundResults[i] = 0;
    }
  }
  if (g_scriptGlobalVars[8] == 1) {
    SaveProfileSetWord(SaveGetProfile(), 2, 0xffffffff);
    return;
  }
  rankedMode = (s32)SaveProfileGetWord(SaveGetProfile(), 7);
  if (rankedMode < 2) {
    if (rankedMode > 0) {
      CoreTaskCreate(0x69, 100);
      SaveProfileSetWord(SaveGetProfile(), 0xb, 0xffffffff);
      if (SaveGetProfileFlag0() == 0) {
        g_actorCrystalSpawnRequest = 1;
      }
      for (i = 0; i < 4; i++) {
        if ((s32)SaveProfileGetWord(SaveGetProfile(), i + 3) > 0) {
          SaveProfileSetWord(SaveGetProfile(), i + 0xe, 0);
        }
      }
    }
  }
  else if (rankedMode < 3) {
    if (seconds == 0xffffffff) {
      SaveProfileSetWord(SaveGetProfile(), 0xb, 0x1e);
    }
    else {
      SaveProfileSetWord(SaveGetProfile(), 0xb, 0xffffffff);
    }
    for (i = 0; i < 4; i++) {
      if ((s32)SaveProfileGetWord(SaveGetProfile(), i + 3) > 0) {
        SaveProfileSetWord(SaveGetProfile(), i + 0xe, 0);
      }
    }
  }
}
