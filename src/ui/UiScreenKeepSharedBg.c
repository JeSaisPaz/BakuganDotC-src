// bdc 0x0890a520 UiScreenKeepSharedBg
#include "bdc.h"

/* Hands the shared screen background over to the next screen: sets the hand-over flag
   `g_uiKeepSharedBg` (so the next `UiScreenCtor` and this screen's `UiScreenDtor` keep
   `g_uiSharedAnims` and its list) and makes sure the shared-background task 320 exists
   (`CoreTaskFind``(0x140)`, else `CoreTaskCreate``(0x140, 100)`). */

void UiScreenKeepSharedBg(UiScreen *screen)

{
  void *task;
  
  g_uiKeepSharedBg = 1;
  task = CoreTaskFind(0x140);
  if (task == (void *)0x0) {
    CoreTaskCreate(0x140,100);
  }
  return;
}

