// bdc 0x089bbe90 BootGetThreadId
#include "bdc.h"

/* Returns the kernel thread id of `g_threadTable` slot `index` (-1 when `index >= 19` or the
   thread is not created). The bound check is signed, so negative indices are not rejected. */

SceUID BootGetThreadId(int index)

{
  SceUID id;

  id = -1;
  if (index < 0x13) {
    id = g_threadTable[index].threadId;
  }
  return id;
}
