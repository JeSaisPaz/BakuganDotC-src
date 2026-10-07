// bdc 0x08909a54 UiSharedBgClose
#include "bdc.h"

/* Closes the shared-background task: finds task id 320 (0x140, `UiSharedBgCtor`) with
   `CoreTaskFind` and sets its close request byte (`closeRequested`), so its update removes it next frame.
   Called by `UiScreenDtor` when no next screen took over the shared background, and by several
   screen phase handlers. */

void UiSharedBgClose(void)

{
  UiScreen *screen;

  screen = (UiScreen *)CoreTaskFind(0x140);
  if (screen != (UiScreen *)0x0) {
    screen->closeRequested = 1;
  }
}
