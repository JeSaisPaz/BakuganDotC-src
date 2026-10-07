// bdc 0x089109ac UiPauseSelectQuitEntry
#include "bdc.h"

/* Puts the pause-menu cursor `+0x78` on the entry whose action is 0xf in the item table of the
   current kind (`g_uiPauseItemActions`, 7 ints per kind) and sets the pending task id
   `g_lastScreenTaskId` = 0x2724. */

void UiPauseSelectQuitEntry(UiPause *self)

{
  int i;

  for (i = 0; i < 7; i++) {
    if (g_uiPauseItemActions[self->kind][i] == 0xf) {
      break;
    }
  }
  self->cursor = i;
  g_lastScreenTaskId = 0x2724;
}
