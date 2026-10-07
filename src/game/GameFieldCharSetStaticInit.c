// bdc 0x088f4f18 GameFieldCharSetStaticInit
#include "bdc.h"

/* Static initialiser (registered in the C++ constructor table at `0x08af5c68..`) of the
   character-set unit: the same 16-bit angle constants at `0x08abf718..0x08abf73c`. */

void GameFieldCharSetStaticInit(void)
{
  g_gameFieldCharAngleTable[0] = 0x71c;
  g_gameFieldCharAngleTable[1] = 0x1555;
  g_gameFieldCharAngleTable[2] = 0x222;
  g_gameFieldCharAngleTable[3] = 0x222;
  g_gameFieldCharAngleTable[4] = 0x2000;
  g_gameFieldCharAngleTable[5] = 0xb6;
  g_gameFieldCharAngleTable[6] = 0x2000;
  g_gameFieldCharAngleTable[7] = 0xb6;
  g_gameFieldCharAngleTable[8] = 0x2000;
  g_gameFieldCharAngleTable[9] = 0xb6;
  g_gameFieldCharAngleTable[10] = 0x2000;
  g_gameFieldCharAngleTable[11] = 0x2d8;
  g_gameFieldCharAngleTable[12] = 0x2000;
  g_gameFieldCharAngleTable[13] = 0xb6;
  g_gameFieldCharAngleTable[14] = 0x4000;
  g_gameFieldCharAngleTable[15] = 0x38e3;
  g_gameFieldCharAngleWord = 0x1555;
  g_gameFieldCharAngleLast = 0x2aaa;
}
