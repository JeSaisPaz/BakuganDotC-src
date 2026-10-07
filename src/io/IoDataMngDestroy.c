// bdc 0x089fce08 IoDataMngDestroy
#include "bdc.h"

/* Destroys the owner-reference pool and the data manager (`CODataMng`, `g_ioDataMng`)
   (`IoDataMngDtor`) and clears the singleton. Called by `BootDataThread` on exit. */

void IoDataMngDestroy(void)

{
  IoDataRefPoolDestroy();
  if (g_ioDataMng != (IoDataMng *)0x0) {
    IoDataMngDtor(g_ioDataMng,3);
    g_ioDataMng = (IoDataMng *)0x0;
  }
  return;
}

