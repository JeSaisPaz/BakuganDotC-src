// bdc 0x088bf1f0 GameFieldPlayStageBgm
#include "bdc.h"

/* Chooses the BGM of the current stage (script variable 1, `g_scriptGlobalVars[1]`, below 0x28) in
   groups of four stages (0..3 → 4, 4..7 → 5, 8..11 → 6, 12..15 → 7, 16..19 → 8, 20..27 → 9,
   32..35 → 3, 36/38/39 → 0x23 or 0x1c when flag 0x1c is set) with story-flag overrides on stages
   0/1 (`CoreBitsetTest` 6 → 0x21, 7 → 0x1c), then queues it on BGM player 0 (`SndBgmQueuePlay`).
   Stages 28..31, 37 and >= 0x28 play nothing. */

void GameFieldPlayStageBgm(void)
{
  u32 stage;
  s32 bgmId;
  bool set;

  stage = (u32)g_scriptGlobalVars[1];
  bgmId = 0;
  if (stage < 0x28) {
    switch (stage) {
    case 0:
    case 1:
    case 2:
    case 3:
      bgmId = 4;
      if (stage == 0) {
        set = CoreBitsetTest(6, g_scriptGlobalBits);
        stage = (u32)g_scriptGlobalVars[1];
        if (set) {
          bgmId = 0x21;
        }
      }
      if (stage == 1 && CoreBitsetTest(7, g_scriptGlobalBits)) {
        bgmId = 0x1c;
      }
      break;
    case 4:
    case 5:
    case 6:
    case 7:
      bgmId = 5;
      break;
    case 8:
    case 9:
    case 10:
    case 11:
      bgmId = 6;
      break;
    case 12:
    case 13:
    case 14:
    case 15:
      bgmId = 7;
      break;
    case 16:
    case 17:
    case 18:
    case 19:
      bgmId = 8;
      break;
    case 20:
    case 21:
    case 22:
    case 23:
      bgmId = 9;
      break;
    case 24:
    case 25:
    case 26:
    case 27:
      bgmId = 9;
      break;
    case 32:
    case 33:
    case 34:
    case 35:
      bgmId = 3;
      break;
    case 36:
    case 38:
    case 39:
      bgmId = 0x23;
      if (CoreBitsetTest(0x1c, g_scriptGlobalBits)) {
        bgmId = 0x1c;
      }
      break;
    default:
      break;
    }
  }
  if (bgmId != 0) {
    SndBgmQueuePlay(0, bgmId, 1, 0);
  }
}
