// bdc 0x088f4808 GameFieldCharSetClearForReload
#include "bdc.h"

/* Calls `GameFieldCharSetClear`; used by `GameFieldUnloadArea`. */

void GameFieldCharSetClearForReload(void *mgr)

{
  GameFieldCharSetClear(mgr);
  return;
}

