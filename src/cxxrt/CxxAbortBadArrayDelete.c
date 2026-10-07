// bdc 0x08a02fa8 CxxAbortBadArrayDelete
#include "bdc.h"

/* Aborts with C++ runtime code 9 (`"freeing array not allocated by an array new operation"`,
   `CxxAbort`). */

void CxxAbortBadArrayDelete(void)

{
  CxxAbort(9);
  return;
}

