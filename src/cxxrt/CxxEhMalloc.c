// bdc 0x08a0347c CxxEhMalloc
#include "bdc.h"

/* `malloc` for the exception-object stack; calls `CxxTerminateInternal` when it fails. */

void *CxxEhMalloc(u32 size)

{
  void *p;

  p = malloc(size);
  if (p == NULL) {
    CxxTerminateInternal();
  }
  return p;
}
