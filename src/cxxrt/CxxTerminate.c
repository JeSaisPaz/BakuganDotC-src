// bdc 0x08a04524 CxxTerminate
#include "bdc.h"

/* `std::terminate()`: calls the installed terminate handler (`0x08af1210`, default
   `CxxDefaultTerminate`) and aborts with code 3 (`"returned from a user-defined terminate()
   routine"`) if it returns. */

void CxxTerminate(void)

{
  if (g_cxxTerminateHandler != NULL) {
    ((void (*)(void))g_cxxTerminateHandler)();
  }
  CxxAbort(3);
}
