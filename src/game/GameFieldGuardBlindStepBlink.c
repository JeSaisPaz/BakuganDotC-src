// bdc 0x088ea258 GameFieldGuardBlindStepBlink
#include "bdc.h"

/* State 1 of the guard-blind timer: lowers word 0x2f by 1 per frame and toggles the view cones and
   0xbd9 gimmicks every 3 frames; at 0 ends the period (`GameFieldGuardBlindEnd`). */

void GameFieldGuardBlindStepBlink(void *blind)
{
  GameFieldGuardBlind *b = (GameFieldGuardBlind *)blind;
  u32 left = SaveProfileGetWord(SaveGetProfile(), 0x2f) - 1;

  if (left == 0) {
    GameFieldGuardBlindEnd(blind);
  } else {
    SaveProfileSetWord(SaveGetProfile(), 0x2f, left);
    b->counter = b->counter + 1;
    if (b->counter % 3 == 0) {
      b->toggle = (b->toggle == 0);
      GameFieldSetGuardConesVisible(blind, b->toggle);
      GameFieldSetGimmicks0bd9B(blind, b->toggle);
    }
  }
}
