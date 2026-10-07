// bdc 0x0880a650 SaveLoadTaskUpdate
#include "bdc.h"

/* Update of the manual load task (id 10010), a step machine on `step`: step 0 (once
   `SysUtilIsInit`) opens the savedata dialog (`SysUtilOpenDialog` kind 1), keeps its handler in
   `g_saveLoadDialogSlot` and issues request 1 (load list) through the handler's virtual slot 2,
   then advances; step 1 waits for a handler and `SysUtilSavedataIsDone`, sets step 999 (finish)
   and, when a profile exists, stores the result in profile word 1: on result 1 it validates the
   loaded data (`SaveProfileIsValid`): valid data is applied (`SaveProfileOnLoaded`), invalid
   data resets the profile, stores 4 instead of the result and goes to step 4; it then closes the
   dialog. Step 4 (once the message window's text box has its printer) shows message 9 of the
   language table in the message window above the active fader (`UiMsgWindowPrintfUtf8`,
   `UiMsgWindowOpen` mode 1) and advances when it opened; step 5 sets step 999 once
   `UiMsgWindowIsClosed`. Any other step removes and destroys the task. */

void SaveLoadTaskUpdate(CoreTask *task)

{
  SaveLoadTask *self = (SaveLoadTask *)task;
  SysUtilSavedataHandler *handler;
  const VtblEntry *entry;
  UiMsgWindow *window;
  GfxFader *fader;
  char *message;
  s32 result;

  switch (self->step) {
  case 0:
    if (SysUtilIsInit()) {
      SysUtilOpenDialog(SysUtilGetCell(), 1);
      *g_saveLoadDialogSlot = SysUtilGetDialog(SysUtilGetCell(), 1);
      handler = (SysUtilSavedataHandler *)*g_saveLoadDialogSlot;
      entry = &handler->base.vtbl[2];
      ((void (*)(void *, s32))entry->fn)((char *)handler + entry->delta, 1);
      self->step++;
    }
    break;
  case 1:
    if (*g_saveLoadDialogSlot != NULL && SysUtilSavedataIsDone()) {
      self->step = 999;
      if (SaveHasProfile()) {
        result = SysUtilSavedataGetResult();
        if (result < 3) {
          if (result <= 0) {
            self->step = 999;
          } else if (result < 2) {
            if (SaveProfileIsValid(SaveGetProfile())) {
              SaveProfileOnLoaded(SaveGetProfile());
            } else {
              result = 4;
              self->step = 4;
              SaveProfileReset(SaveGetProfile());
            }
          }
        } else if (result != 5) {
          self->step = 999;
        }
        SaveProfileSetWord(SaveGetProfile(), 1, (u32)result);
      }
      SysUtilCloseDialog(SysUtilGetCell(), 1);
    }
    break;
  case 4:
    window = (UiMsgWindow *)UiMsgWindowGet();
    if (UiTextBoxHasPrinter((UiTextBox *)window->textBox)) {
      window = (UiMsgWindow *)UiMsgWindowGet();
      fader = GfxGetActiveFader();
      window->depth = fader->sortKey + 1.0f;
      window = (UiMsgWindow *)UiMsgWindowGet();
      message = g_langStrings[9];
      UiMsgWindowPrintfUtf8(window, message);
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 1;
      if (UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 1, -1) != 0) {
        self->step++;
      }
    }
    break;
  case 5:
    if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
      self->step = 999;
    }
    break;
  default:
    CoreTaskRemove(task, true);
    break;
  }
}
