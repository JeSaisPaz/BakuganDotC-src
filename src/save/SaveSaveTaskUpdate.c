// bdc 0x0880b23c SaveSaveTaskUpdate
#include "bdc.h"

/* Update of the manual save task (id 10020), a step machine on `step`. Steps 0, 2 and 6 (once
   `SysUtilIsInit`; step 6 also waits for the active fader) open the savedata dialog
   (`SysUtilOpenDialog` kind 1), keep its handler in `g_saveSaveDialogSlot` and issue a request
   through the handler's virtual slot 2: 0 the save list (then step 1), 4 the size check (then
   step 3), 2 the delete list (then step 7). Steps 1, 3 and 7 wait for a handler and
   `SysUtilSavedataIsDone`, store the result in profile word 1 (steps 1 and 3, when a profile
   exists, else the result counts as 0) and close the dialog: step 1 on result 1 sets step 10
   (finish), profile flag `0x40000000` and bit 2 of profile word 0x35 (`UiMenuFlagsModify(1, 2)`),
   else goes to step 2; step 3 goes to step 4 on result 7..8 ('not enough space'), else step 8;
   step 7 retries from step 0. Step 4 (once the message window's text box has its printer) puts
   the window above the active fader, prints language string 2 ('needs %d KB') with
   `SysUtilGetSaveRequiredBytes`` >> 10` into it and opens it (mode 1); when it opened, step 5
   and a 30-frame fader swap (preset 4). Step 5 waits for the fader and the window to close:
   choice 0 goes to step 6 (delete list to make room), any other to step 8, each with the fader
   swap. Step 8 (after the fader) shows language string 4 the same way and goes to step 9; step 9
   on choice 0 sets step 10 with the fader swap, any other choice retries from step 0. Any other
   step (10 included) removes and destroys the task. */

void SaveSaveTaskUpdate(CoreTask *task)

{
  SaveSaveTask *self = (SaveSaveTask *)task;
  SysUtilSavedataHandler *handler;
  const VtblEntry *entry;
  UiMsgWindow *window;
  GfxFader *fader;
  char *format;
  s32 result;
  char text[256];

  switch (self->step) {
  case 0:
    if (SysUtilIsInit()) {
      SysUtilOpenDialog(SysUtilGetCell(), 1);
      *g_saveSaveDialogSlot = SysUtilGetDialog(SysUtilGetCell(), 1);
      handler = (SysUtilSavedataHandler *)*g_saveSaveDialogSlot;
      entry = &handler->base.vtbl[2];
      ((void (*)(void *, s32))entry->fn)((char *)handler + entry->delta, 0);
      self->step = 1;
    }
    break;
  case 1:
    if (*g_saveSaveDialogSlot != NULL && SysUtilSavedataIsDone()) {
      result = 0;
      if (SaveHasProfile()) {
        result = SysUtilSavedataGetResult();
        SaveProfileSetWord(SaveGetProfile(), 1, (u32)result);
      }
      SysUtilCloseDialog(SysUtilGetCell(), 1);
      if (result == 1) {
        self->step = 10;
        SaveProfileSetFlags(SaveGetProfile(), 0x40000000);
        UiMenuFlagsModify(1, 2);
      } else {
        self->step = 2;
      }
    }
    break;
  case 2:
    if (SysUtilIsInit()) {
      SysUtilOpenDialog(SysUtilGetCell(), 1);
      *g_saveSaveDialogSlot = SysUtilGetDialog(SysUtilGetCell(), 1);
      handler = (SysUtilSavedataHandler *)*g_saveSaveDialogSlot;
      entry = &handler->base.vtbl[2];
      ((void (*)(void *, s32))entry->fn)((char *)handler + entry->delta, 4);
      self->step = 3;
    }
    break;
  case 3:
    if (*g_saveSaveDialogSlot != NULL && SysUtilSavedataIsDone()) {
      result = 0;
      if (SaveHasProfile()) {
        result = SysUtilSavedataGetResult();
        SaveProfileSetWord(SaveGetProfile(), 1, (u32)result);
      }
      SysUtilCloseDialog(SysUtilGetCell(), 1);
      if (result < 7 || result > 8) {
        self->step = 8;
      } else {
        self->step = 4;
      }
    }
    break;
  case 4:
    window = (UiMsgWindow *)UiMsgWindowGet();
    if (UiTextBoxHasPrinter((UiTextBox *)window->textBox)) {
      window = (UiMsgWindow *)UiMsgWindowGet();
      fader = GfxGetActiveFader();
      window->depth = fader->sortKey + 1.0f;
      format = g_langStrings[2];
      sprintf(text, format, SysUtilGetSaveRequiredBytes() >> 10);
      UiMsgWindowPrintfUtf8((UiMsgWindow *)UiMsgWindowGet(), text);
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 0;
      if (UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 1, -1) != 0) {
        self->step = 5;
        GfxFaderSetPreset(GfxGetActiveFader(), 4);
        GfxFaderStart(GfxGetActiveFader(), 30);
      }
    }
    break;
  case 5:
    if (GfxFaderIsFinished(GfxGetActiveFader()) &&
        UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
      if (UiMsgWindowGetChoice((UiMsgWindow *)UiMsgWindowGet()) == 0) {
        self->step = 6;
      } else {
        self->step = 8;
      }
    }
    if (self->step != 5) {
      GfxFaderSetPreset(GfxGetActiveFader(), 4);
      GfxFaderStart(GfxGetActiveFader(), 30);
    }
    break;
  case 6:
    if (GfxFaderIsFinished(GfxGetActiveFader()) && SysUtilIsInit()) {
      SysUtilOpenDialog(SysUtilGetCell(), 1);
      *g_saveSaveDialogSlot = SysUtilGetDialog(SysUtilGetCell(), 1);
      handler = (SysUtilSavedataHandler *)*g_saveSaveDialogSlot;
      entry = &handler->base.vtbl[2];
      ((void (*)(void *, s32))entry->fn)((char *)handler + entry->delta, 2);
      self->step = 7;
    }
    break;
  case 7:
    if (*g_saveSaveDialogSlot != NULL && SysUtilSavedataIsDone()) {
      SysUtilCloseDialog(SysUtilGetCell(), 1);
      self->step = 0;
    }
    break;
  case 8:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      window = (UiMsgWindow *)UiMsgWindowGet();
      if (UiTextBoxHasPrinter((UiTextBox *)window->textBox)) {
        window = (UiMsgWindow *)UiMsgWindowGet();
        fader = GfxGetActiveFader();
        window->depth = fader->sortKey + 1.0f;
        window = (UiMsgWindow *)UiMsgWindowGet();
        UiMsgWindowPrintfUtf8(window, g_langStrings[4]);
        ((UiMsgWindow *)UiMsgWindowGet())->mode = 0;
        if (UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 1, -1) != 0) {
          self->step = 9;
        }
      }
    }
    break;
  case 9:
    if (GfxFaderIsFinished(GfxGetActiveFader()) &&
        UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
      if (UiMsgWindowGetChoice((UiMsgWindow *)UiMsgWindowGet()) != 0) {
        self->step = 0;
      } else {
        self->step = 10;
      }
    }
    if (self->step == 10) {
      GfxFaderSetPreset(GfxGetActiveFader(), 4);
      GfxFaderStart(GfxGetActiveFader(), 30);
    }
    break;
  default:
    CoreTaskRemove(task, true);
    break;
  }
}
