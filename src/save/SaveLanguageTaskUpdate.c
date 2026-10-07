// bdc 0x0880aa70 SaveLanguageTaskUpdate
#include "bdc.h"

/* Update of the language-file save task (id 10021), a step machine on `step` like
   `SaveSaveTaskUpdate` but for `LANGUAGE.BIN`. Steps 0, 2 and 6 (once `SysUtilIsInit`; steps 0
   and 6 also wait for the active fader) open the savedata dialog (`SysUtilOpenDialog` kind 1),
   keep its handler in `g_saveLanguageDialogSlot` and issue a request through the handler's
   virtual slot 2: 10 the language-file save (then step 1), 6 the size check (then step 3), 2 the
   delete list (then step 7). Steps 1, 3 and 7 wait for a handler and `SysUtilSavedataIsDone`,
   store the result in profile word 1 (steps 1 and 3, when a profile exists, else the result counts
   as 0) and close the dialog: step 1 on result 1 sets step 10 (finish), else goes to step 2; step
   3 goes to step 4 on result 7..8 ('not enough space'), else step 8; step 7 retries from step 0.
   Step 4 (once the message window's text box has its printer) puts the window above the active
   fader, prints language string 2 ('needs %d KB') with `SysUtilGetSaveRequiredBytes`` >> 10`
   into it and opens it (mode 1); when it opened, step 5 and a 30-frame fader swap (preset 4).
   Step 5 waits for the fader and the window to close: choice 0 goes to step 6 (delete list to
   make room), any other to step 8, each with the fader swap. Step 8 (after the fader) puts the
   window below the fader, shows language string 3 the same way and goes to step 9 when it opened,
   with the fader swap; step 9 on choice 0 sets step 10, any other choice retries from step 0,
   each with the fader swap. Any other step (10 included) removes and destroys the task. */

void SaveLanguageTaskUpdate(CoreTask *task)

{
  SaveLanguageTask *self = (SaveLanguageTask *)task;
  SysUtilSavedataHandler *handler;
  const VtblEntry *entry;
  UiMsgWindow *window;
  GfxFader *fader;
  char *format;
  s32 result;
  char text[256];

  switch (self->step) {
  case 0:
    if (GfxFaderIsFinished(GfxGetActiveFader()) && SysUtilIsInit()) {
      SysUtilOpenDialog(SysUtilGetCell(), 1);
      *g_saveLanguageDialogSlot = SysUtilGetDialog(SysUtilGetCell(), 1);
      handler = (SysUtilSavedataHandler *)*g_saveLanguageDialogSlot;
      entry = &handler->base.vtbl[2];
      ((void (*)(void *, s32))entry->fn)((char *)handler + entry->delta, 10);
      self->step = 1;
    }
    break;
  case 1:
    if (*g_saveLanguageDialogSlot != NULL && SysUtilSavedataIsDone()) {
      result = 0;
      if (SaveHasProfile()) {
        result = SysUtilSavedataGetResult();
        SaveProfileSetWord(SaveGetProfile(), 1, (u32)result);
      }
      SysUtilCloseDialog(SysUtilGetCell(), 1);
      if (result == 1) {
        self->step = 10;
      } else {
        self->step = 2;
      }
    }
    break;
  case 2:
    if (SysUtilIsInit()) {
      SysUtilOpenDialog(SysUtilGetCell(), 1);
      *g_saveLanguageDialogSlot = SysUtilGetDialog(SysUtilGetCell(), 1);
      handler = (SysUtilSavedataHandler *)*g_saveLanguageDialogSlot;
      entry = &handler->base.vtbl[2];
      ((void (*)(void *, s32))entry->fn)((char *)handler + entry->delta, 6);
      self->step = 3;
    }
    break;
  case 3:
    if (*g_saveLanguageDialogSlot != NULL && SysUtilSavedataIsDone()) {
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
      *g_saveLanguageDialogSlot = SysUtilGetDialog(SysUtilGetCell(), 1);
      handler = (SysUtilSavedataHandler *)*g_saveLanguageDialogSlot;
      entry = &handler->base.vtbl[2];
      ((void (*)(void *, s32))entry->fn)((char *)handler + entry->delta, 2);
      self->step = 7;
    }
    break;
  case 7:
    if (*g_saveLanguageDialogSlot != NULL && SysUtilSavedataIsDone()) {
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
        window->depth = fader->sortKey - 1.0f;
        window = (UiMsgWindow *)UiMsgWindowGet();
        UiMsgWindowPrintfUtf8(window, g_langStrings[3]);
        ((UiMsgWindow *)UiMsgWindowGet())->mode = 0;
        if (UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 1, -1) != 0) {
          self->step = 9;
        }
      }
    }
    if (self->step != 8) {
      GfxFaderSetPreset(GfxGetActiveFader(), 4);
      GfxFaderStart(GfxGetActiveFader(), 30);
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
    if (self->step != 9) {
      GfxFaderSetPreset(GfxGetActiveFader(), 4);
      GfxFaderStart(GfxGetActiveFader(), 30);
    }
    break;
  default:
    CoreTaskRemove(task, true);
    break;
  }
}
