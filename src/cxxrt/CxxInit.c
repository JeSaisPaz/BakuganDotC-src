// bdc 0x08a029bc CxxInit
#include "bdc.h"

/* First call of `main`: runs the C++ static constructors by calling `CxxRunStaticCtors`.
   Nothing else (the destructor/`atexit` side of the runtime is not called here). */

void CxxInit(void)

{
  CxxRunStaticCtors();
  return;
}

