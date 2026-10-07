// bdc 0x088eab68 GameEventClearBlockD40
#include "bdc.h"

/* Clears the 0x9c-byte block `0x08b00d40`; called next to `GameEventStateClear`. */

void GameEventClearBlockD40(void)

{
  memset(&g_gameEventLocationBlock,0,0x9c);
  return;
}

