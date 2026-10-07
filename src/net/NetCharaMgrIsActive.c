// bdc 0x089cf658 NetCharaMgrIsActive
#include "bdc.h"

/* Returns 1 when the `CONetChara` manager exists and its active byte `0x08ac593c` is set
   (`NetCharaMgrCreate`), else 0. */

int NetCharaMgrIsActive(void)

{
  int result;
  
  result = 0;
  if ((g_netCharaMgrActive != '\0') && (g_netCharaMgr != (void *)0x0)) {
    result = 1;
  }
  return result;
}

