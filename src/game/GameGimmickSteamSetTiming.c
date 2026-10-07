// bdc 0x088d5c50 GameGimmickSteamSetTiming
#include "bdc.h"

/* Sets the off/on durations of the steam gimmick (`GameGimmickSteamCtor`, vtables
   `0x08af2edc`/`0x08af2f7c`) (`+0x184 = 90`, `+0x188 = 240` frames); every record parameter (0xe,
   0x14, 0x15, 0x19, default) uses the same values. */

void GameGimmickSteamSetTiming(GameGimmickSteam *obj)
{
  obj->onFrames = 0x5a;
  obj->offFrames = 0xf0;
}
