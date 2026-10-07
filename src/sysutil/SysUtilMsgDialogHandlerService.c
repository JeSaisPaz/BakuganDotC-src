// bdc 0x08a01414 SysUtilMsgDialogHandlerService
#include "bdc.h"

/* Service virtual (slot `+0x24`, polled by `SysUtilPoll`) of the PSP message-dialog handler
   (error dialog, vtable `0x08af5a14`; params block in `g_msgDialogParams`): drives the busy
   word: 1 -> `sceUtilityMsgDialogInitStart` when the utility state is not 1..4 (-> 2 on success);
   3 -> `sceUtilityMsgDialogAbort` while the dialog is visible (state 2, -> 4 on success);
   `sceUtilityMsgDialogShutdownStart` when the utility asks to quit (state 3, -> 4 on success);
   busy 4 and utility back to state 0 -> busy 0, handler disabled. Tracks the busy word in
   `g_msgDialogLastBusy`, polls `SysUtilMsgDialogIsIdle` and always returns 0. */

int SysUtilMsgDialogHandlerService(SysUtilHandler *self)
{
  s32 busy;
  s32 state;

  busy = g_msgDialogParams->busy;
  if (busy < 2) {
    if (busy > 0) {
      state = SysUtilHandlerGetState(self);
      if ((state < 1 || state > 4) &&
          sceUtilityMsgDialogInitStart(&g_msgDialogParams->params) == 0) {
        g_msgDialogParams->busy = 2;
      }
    }
  }
  else if (busy < 5) {
    state = SysUtilHandlerGetState(self);
    if (state < 2) {
      if (state == 0 && g_msgDialogParams->busy == 4) {
        g_msgDialogParams->busy = 0;
        SysUtilHandlerSetEnabled(self, 0);
      }
    }
    else if (state < 3) {
      if (g_msgDialogParams->busy == 3 && sceUtilityMsgDialogAbort() == 0) {
        g_msgDialogParams->busy = 4;
      }
    }
    else if (state < 4) {
      if (sceUtilityMsgDialogShutdownStart() == 0) {
        g_msgDialogParams->busy = 4;
      }
    }
  }
  if (g_msgDialogParams->busy != g_msgDialogLastBusy) {
    g_msgDialogLastBusy = g_msgDialogParams->busy;
  }
  SysUtilMsgDialogIsIdle();
  return 0;
}
