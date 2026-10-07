// bdc 0x0882cfd4 UiTalkSetCloseRequest
#include "bdc.h"

/* Sets the close-request word `+0x64c` of the talk/HUD task (`UiGetTalkTask`) (id 0x6e,
   `BtlHudUpdate`) (the same flag `UiTalkRequestClose` sets). Called by `BtlMainPhaseWaitHud`.
    */

void UiTalkSetCloseRequest(BtlHud *win)

{
  win->msgStep = 1;
  return;
}

