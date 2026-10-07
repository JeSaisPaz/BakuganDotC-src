// bdc 0x08950908 UiTitleMenuExitPhase
#include "bdc.h"

/* Exit phase (entry 3 of the phase table `0x08a9d3b8`) of screen task 1000 (`UiTitleMenuCtor`;
   loads `"data/sysmenu.lzs"` and `"title.fab"`): sets `closeRequested` (`+0x4c`). */

void UiTitleMenuExitPhase(UiScreen *screen)

{
  screen->closeRequested = '\x01';
  return;
}

