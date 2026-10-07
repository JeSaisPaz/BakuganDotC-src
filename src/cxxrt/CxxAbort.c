// bdc 0x08a02820 CxxAbort
#include "bdc.h"

/* Fatal exit of the C++ runtime: prints the message for `code` (`CxxPrintAbortMessage`), calls
   `abort` and, should that ever return, spins forever. Never returns. Callers:
   `CxxAbortDoubleRegister` (code 4), `CxxAbortBadArrayDelete` (code 9), `CxxDefaultTerminate`
   (code 2) and `CxxTerminate` (code 3). */

void CxxAbort(int code)

{
  CxxPrintAbortMessage(code);
  abort();
  for (;;) {
  }
}

