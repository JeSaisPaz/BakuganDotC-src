// bdc 0x089a6c50 UiMainMenuShouldStartBgm
#include "bdc.h"

/* Decides whether the main menu must (re)start its BGM: returns 1 when the last closed screen's task
   id (`g_lastScreenTaskId`) is 100, 200, 0x136, 0x137, 0x19a or 3004, or when menu flag 0
   (`UiMenuFlagsTest`) is set; always clears the menu flags afterwards (`UiMenuFlagsModify``(2,
   0)`). Used by `UiMainMenuPhaseLoad` before queuing track 0x17. */

int UiMainMenuShouldStartBgm(void)
{
  int start;

  if (g_lastScreenTaskId < 0x136) {
    if (g_lastScreenTaskId < 0x65) {
      start = g_lastScreenTaskId >= 100;
    } else {
      start = g_lastScreenTaskId == 200;
    }
  } else if (g_lastScreenTaskId < 0x19b) {
    start = g_lastScreenTaskId < 0x138 || g_lastScreenTaskId >= 0x19a;
  } else {
    start = g_lastScreenTaskId == 0xbbc;
  }
  if (!start && UiMenuFlagsTest(0)) {
    start = 1;
  }
  UiMenuFlagsModify(2, 0);
  return start;
}
