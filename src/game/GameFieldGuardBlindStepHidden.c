// bdc 0x088ea184 GameFieldGuardBlindStepHidden
#include "bdc.h"

/* State 0 of the guard-blind timer: lowers save-profile word 0x2f by 2 per frame; once it would
   drop below 93 switches to the blink state. */

void GameFieldGuardBlindStepHidden(void *blind)
{
  u32 value = SaveProfileGetWord(SaveGetProfile(), 0x2f);
  s32 next = (s32)(value - 1) - 1;

  if (next < 0x5b) {
    GameFieldGuardBlindEnterBlinkThunk(blind);
  } else {
    SaveProfileSetWord(SaveGetProfile(), 0x2f, next);
  }
}
