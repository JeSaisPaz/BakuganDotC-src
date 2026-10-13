// bdc 0x0882cf60 UiTalkRequestClose
#include "bdc.h"

/* Requests closing the talk window (talk/HUD task 0x6e, the `BtlHud` `win`): clears UI window
   flag 0xb (`UiSetWindowActive`), stores `value` in `msgIndex` (`+0x658`) and -1 in `msgAux`
   (`+0x65c`); then, unless window flag 0xc is set, raises the script message step `msgStep = 1`
   (`+0x64c`, `BtlHudUpdateScriptMessage` then shows message `value`) directly; with flag 0xc set
   it instead switches the battle main task (id 100, `BtlGetCameraTask`) into its wait-for-HUD
   phase (`BtlMainEnterWaitHud`), whose handler `BtlMainPhaseWaitHud` raises the same word
   through `UiTalkSetCloseRequest`. */

void UiTalkRequestClose(void *win, s32 value)

{
  BtlHud *hud = (BtlHud *)win;

  UiSetWindowActive(0xb,0);
  hud->msgIndex = value;
  hud->msgAux = -1;
  if (UiGetWindowActive(0xc) == 0) {
    hud->msgStep = 1;
  }
  else if (BtlCameraTaskExists() != 0) {
    BtlMainEnterWaitHud(BtlGetCameraTask());
  }
}
