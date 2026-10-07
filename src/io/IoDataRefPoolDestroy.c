// bdc 0x089fb6cc IoDataRefPoolDestroy
#include "bdc.h"

/* Destroys the owner-reference pool `g_ioDataRefPool` (`MemPoolDestroy`) and clears it. Called
   by `IoDataMngDestroy`. */

void IoDataRefPoolDestroy(void)

{
  if (g_ioDataRefPool != (MemPool *)0x0) {
    MemPoolDestroy(g_ioDataRefPool,3);
    g_ioDataRefPool = (MemPool *)0x0;
  }
  return;
}

