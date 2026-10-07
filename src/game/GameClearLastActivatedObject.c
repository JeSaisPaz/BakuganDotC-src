// bdc 0x088a8ed8 GameClearLastActivatedObject
#include "bdc.h"

/* Clears `g_gameLastActivatedObject` (stores 0) (singleton accessor, named by `bdc singleton`).
    */

void GameClearLastActivatedObject(void)

{
  g_gameLastActivatedObject = (void *)0x0;
  return;
}

