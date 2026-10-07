// bdc 0x0882cf60 UiTalkRequestClose
#include "bdc.h"

/* Requests closing the talk window (talk/HUD task 0x6e, `win`): clears UI window flag 0xb
   (`UiSetWindowActive`), stores `value` at `win+0x658` and -1 at `win+0x65c`; then, unless window
   flag 0xc is set, raises the close-request word `win+0x64c = 1` directly; with flag 0xc set it
   instead switches the battle main task (id 100, `BtlGetCameraTask`) into its wait-for-HUD phase
   (`BtlMainEnterWaitHud`), whose handler `BtlMainPhaseWaitHud` raises the same word through
   `UiTalkSetCloseRequest`. */

void UiTalkRequestClose(void *win, s32 value)

{
  UiTalkTask *task = (UiTalkTask *)win;

  UiSetWindowActive(0xb,0);
  task->closeValue = value;
  task->closeAux = -1;
  if (UiGetWindowActive(0xc) == 0) {
    task->closeRequest = 1;
  }
  else if (BtlCameraTaskExists() != 0) {
    BtlMainEnterWaitHud(BtlGetCameraTask());
  }
}
