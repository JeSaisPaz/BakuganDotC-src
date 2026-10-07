// bdc 0x089c85d4 SndBgmCmdListCreate
#include "bdc.h"

/* Creates the global list of BGM commands `g_sndBgmCmdList` if it does not exist yet: allocates
   the 0x10-byte `CoreList` header from the low heap and constructs it with
   `SndBgmCmdListInit``(list, capacity)`. Does nothing (no store) when the list already exists;
   if the allocation fails the global is set to NULL. */

void SndBgmCmdListCreate(s32 capacity)
{
  bool fromLow;
  CoreList *list;

  if (g_sndBgmCmdList != NULL) {
    return;
  }
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  list = MemAlloc(sizeof(CoreList), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (list != NULL) {
    SndBgmCmdListInit(list, capacity);
  }
  g_sndBgmCmdList = list;
}
