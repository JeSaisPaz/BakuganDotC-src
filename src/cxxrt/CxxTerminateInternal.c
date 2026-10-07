// bdc 0x08a044f0 CxxTerminateInternal
#include "bdc.h"

/* Runtime-internal terminate: sets the in-terminate flag `0x08af1218`, calls `std::terminate`
   (`CxxTerminate`) and `abort`s. */

void CxxTerminateInternal(void)

{
  g_cxxInTerminate = 1;
  CxxTerminate();
  abort();
  for (;;) {
  }
}
