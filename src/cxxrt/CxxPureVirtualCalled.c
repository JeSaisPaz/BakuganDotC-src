// bdc 0x08a0284c CxxPureVirtualCalled
#include "bdc.h"

/* Default body of a pure virtual function slot: calls `abort` and never returns. */

void CxxPureVirtualCalled(void)
{
  abort();
  for (;;) {
  }
}
