// bdc 0x08a026a8 CxxAbortDoubleRegister
#include "bdc.h"

/* Thin wrapper `CxxAbort(4)`: aborts with "internal error: static object marked for destruction
   more than once". Its only caller is `CxxRegisterGlobalObject`. */

void CxxAbortDoubleRegister(void)

{
  CxxAbort(4);
  return;
}

