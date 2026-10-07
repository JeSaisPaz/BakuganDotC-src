// bdc 0x088f27bc GameEvent470StaticInit
#include "bdc.h"

/* Static initialiser (registered in the C++ constructor table at `0x08af5c68..`) of the task-470
   event unit: the 16-bit angle/turn constants at `0x08abf6e8..0x08abf70c`. */

void GameEvent470StaticInit(void)

{
  g_event470AngleTable[0] = 0x71c;
  g_event470AngleTable[1] = 0x1555;
  g_event470AngleTable[2] = 0x222;
  g_event470AngleTable[3] = 0x222;
  g_event470AngleTable[4] = 0x2000;
  g_event470AngleTable[5] = 0xb6;
  g_event470AngleTable[6] = 0x2000;
  g_event470AngleTable[7] = 0xb6;
  g_event470AngleTable[8] = 0x2000;
  g_event470AngleTable[9] = 0xb6;
  g_event470AngleTable[10] = 0x2000;
  g_event470AngleTable[11] = 0x2d8;
  g_event470AngleTable[12] = 0x2000;
  g_event470AngleTable[13] = 0xb6;
  g_event470AngleTable[14] = 0x4000;
  g_event470AngleTable[15] = 0x38e3;
  g_event470TurnStep = 0x1555;
  g_event470TurnMax = 0x2aaa;
  return;
}
