// bdc 0x08a04558 CxxUnexpected
#include "bdc.h"

/* `std::unexpected()`: calls the installed unexpected handler (`0x08af1214`) if any, then
   `CxxTerminate`. */

void CxxUnexpected(void)

{
  if (g_cxxUnexpectedHandler != 0) {
    ((void (*)(void))g_cxxUnexpectedHandler)();
  }
  CxxTerminate();
  return;
}
