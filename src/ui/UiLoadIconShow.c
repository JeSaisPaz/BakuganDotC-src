// bdc 0x0880908c UiLoadIconShow
#include "bdc.h"

/* Shows the 'now loading' icon, creating it first if needed: when `g_loadIcon` is NULL it
   allocates the 0x20-byte object from the low heap, constructs it (`UiLoadIconInit`), stores it
   in the global and registers it as a task at priority 0 (`CoreTaskInsert`); then sets it visible
   (`UiLoadIconSetVisible``(icon, 1)`). Called by `GfxInitBootResources` (which hides it again
   right away) and one other caller. */

void UiLoadIconShow(void)

{
  bool fromLow;
  void *this;
  void *task;
  
  if (g_loadIcon == (void *)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    this = MemAlloc(0x20,(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    task = (void *)0x0;
    if (this != (void *)0x0) {
      UiLoadIconInit(this);
      task = this;
    }
    g_loadIcon = task;
    CoreTaskInsert(task,0);
    UiLoadIconSetVisible(g_loadIcon,'\x01');
  }
  else {
    UiLoadIconSetVisible(g_loadIcon,'\x01');
  }
  return;
}

