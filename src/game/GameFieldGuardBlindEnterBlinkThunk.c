// bdc 0x088ea168 GameFieldGuardBlindEnterBlinkThunk
#include "bdc.h"

/* Calls `GameFieldGuardBlindEnterBlink`. */
void GameFieldGuardBlindEnterBlinkThunk(void *blind)
{
    GameFieldGuardBlindEnterBlink(blind);
}
