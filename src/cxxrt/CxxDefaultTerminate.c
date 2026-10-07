// bdc 0x08a04384 CxxDefaultTerminate
#include "bdc.h"

/* Default terminate handler: aborts with C++ runtime code 2 (`"terminate() called by the exception
   handling mechanism"`). */

void CxxDefaultTerminate(void)

{
  CxxAbort(2);
  return;
}

