// bdc 0x0880baf0 SaveAutoSaveTaskUpdate
#include "bdc.h"

/* Update of the autosave task (id 10022), a step machine on `step`; any step >= 18 (18 = done)
   removes and destroys the task and returns at once.
   Step 0: with a known slot (`SysUtilGetSaveSlotIndex` >= 0) sets `hasSlot` and step 1, else
   clears `hasSlot` and goes to step 15 (retry prompt); clears `timer`.
   Step 1 (once `SysUtilIsInit`) opens the savedata dialog (kind 1) into `handler`; with a slot it
   issues request 13 through the handler's virtual slot 2, sets a 45-frame `timer` and step 4;
   without one it sets up the private fader (colour/start/end {0.267, 0.533, 0.267, 0}, end alpha
   0.7, sort key 1900, preset 3, 15 frames, enabled) and goes to step 2. Both clear `dialogResult`.
   Step 2 waits for the fader, then issues request 0 (save list) and goes to step 3; step 3 waits
   for `SysUtilSavedataIsDone`, swaps the fader back (preset 4, 15 frames) and goes to step 4.
   Step 4 waits for the fader and a handler, counts `timer` down to 1 (then waits for the request
   and zeroes it); with `timer <= 0` and the request done it stores `SysUtilSavedataGetResult` in
   profile word 1 (when a profile exists), keeps `SysUtilHandlerGetResult` in `dialogResult`,
   closes the dialog and clears `handler`. Result 1, or 2 with a slot, finishes (step 18);
   otherwise without a slot it goes to step 8 (size check), with one by result: 4 -> step 6,
   5 -> step 7, 7..8 -> step 8, anything else -> step 5.
   Steps 5, 6, 7, 17 and 15 (once the message window's text box has its printer) print language
   string 5 (formatted with `dialogResult`), 6, 7, 4 and 8 into the message window, set its mode 0
   and open it (mode 1); when it opened they go to step 14 (15: step 16).
   Step 8 opens the dialog and issues request 4 (size check), step 9; step 9 waits for a handler
   and the request, stores the result in profile word 1 (only with a profile, else it counts as 0),
   closes the dialog and goes to step 10 on result 7..8 ('not enough space'), else step 17.
   Step 10 prints language string 2 with `SysUtilGetSaveRequiredBytes` / 1024 (at least 1) and
   opens the window, step 11; step 11 on choice 0 goes to step 12, else step 17. Step 12 issues
   request 2 (delete list), step 13; step 13 waits for it, closes the dialog and restarts at step 1.
   Step 14 waits for the window: choice 0 finishes (step 18), else step 15 when `unk21` is set,
   step 1 otherwise. Step 16: choice 0 clears `unk21` and retries (step 1), any other sets `unk21`
   and goes to step 17.
   Every step < 18 then steps the fader (when there is one) and runs the text box (when there is
   one) on `unk30`: 0 creates its printer (32 chars), font 1, white colour and outline colour, ->1;
   1 while `timer > 0` prints language string 10 at (32, 240), clears `textFrame`, ->2; 2 counts
   `textFrame`, clears the box and goes back to 1 once `timer` is 0, else bobs every glyph's y to
   240 + 3 * cos((2 * textFrame + 8 * i) * pi / 15). */

static void SaveAutoSaveTaskRequest(SysUtilSavedataHandler *handler, s32 request)
{
  const VtblEntry *entry = &handler->base.vtbl[2];

  ((void (*)(void *, s32))entry->fn)((char *)handler + entry->delta, request);
}

void SaveAutoSaveTaskUpdate(CoreTask *task)

{
  SaveAutoSaveTask *self = (SaveAutoSaveTask *)task;
  GfxFader *fader;
  UiTextPrinter *printer;
  GfxSprite *glyph;
  s32 result;
  s32 kb;
  s32 count;
  s32 i;
  s32 phase;
  float angle;
  float c;
  char text[256];
  char kbText[256];
  u8 encoded[56];

  switch (self->step) {
  case 0:
    if (SysUtilGetSaveSlotIndex() < 0) {
      self->step = 15;
      self->hasSlot = 0;
    } else {
      self->step = 1;
      self->hasSlot = 1;
    }
    self->timer = 0;
    break;
  case 1:
    if (!SysUtilIsInit()) {
      break;
    }
    SysUtilOpenDialog(SysUtilGetCell(), 1);
    self->handler = (SysUtilSavedataHandler *)SysUtilGetDialog(SysUtilGetCell(), 1);
    if (self->hasSlot) {
      SaveAutoSaveTaskRequest(self->handler, 13);
      self->step = 4;
      self->timer = 45;
    } else {
      fader = self->fader;
      fader->color[0] = 0.26667f;
      fader->color[1] = 0.53333f;
      fader->color[2] = 0.26667f;
      fader->color[3] = 0.0f;
      for (i = 0; i < 4; i++) {
        self->fader->start[i] = self->fader->color[i];
      }
      for (i = 0; i < 4; i++) {
        self->fader->end[i] = self->fader->color[i];
      }
      self->fader->end[3] = 0.7f;
      self->fader->sortKey = 1900.0f;
      GfxFaderSetPreset(self->fader, 3);
      GfxFaderStart(self->fader, 15);
      GfxFaderSetEnabled(self->fader, 1);
      self->step = 2;
    }
    self->dialogResult = 0;
    break;
  case 2:
    if (GfxFaderIsFinished(self->fader)) {
      SaveAutoSaveTaskRequest(self->handler, 0);
      self->step = 3;
    }
    break;
  case 3:
    if (SysUtilSavedataIsDone()) {
      GfxFaderSetPreset(self->fader, 4);
      GfxFaderStart(self->fader, 15);
      self->step = 4;
    }
    break;
  case 4:
    if (!GfxFaderIsFinished(self->fader) || self->handler == NULL) {
      break;
    }
    if (self->timer > 0) {
      if (self->timer >= 2) {
        self->timer = self->timer - 1;
      } else if (SysUtilSavedataIsDone()) {
        self->timer = 0;
      }
      break;
    }
    if (!SysUtilSavedataIsDone()) {
      break;
    }
    result = SysUtilSavedataGetResult();
    self->dialogResult = SysUtilHandlerGetResult((pspUtilityDialogCommon **)self->handler);
    if (SaveHasProfile()) {
      SaveProfileSetWord(SaveGetProfile(), 1, (u32)result);
    }
    SysUtilCloseDialog(SysUtilGetCell(), 1);
    self->handler = NULL;
    if (result == 1) {
      self->step = 18;
    } else if (result == 2) {
      self->step = self->hasSlot ? 18 : 17;
    }
    if (self->step == 18) {
      break;
    }
    if (!self->hasSlot) {
      self->step = 8;
    } else if (result == 4) {
      self->step = 6;
    } else if (result == 5) {
      self->step = 7;
    } else if (result >= 7 && result <= 8) {
      self->step = 8;
    } else {
      self->step = 5;
    }
    break;
  case 5:
    if (UiTextBoxHasPrinter((UiTextBox *)((UiMsgWindow *)UiMsgWindowGet())->textBox)) {
      sprintf(text, g_langStrings[5], self->dialogResult);
      UiMsgWindowPrintfUtf8((UiMsgWindow *)UiMsgWindowGet(), text);
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 0;
      if (UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 1, -1) != 0) {
        self->step = 14;
      }
    }
    break;
  case 6:
    if (UiTextBoxHasPrinter((UiTextBox *)((UiMsgWindow *)UiMsgWindowGet())->textBox)) {
      UiMsgWindowPrintfUtf8((UiMsgWindow *)UiMsgWindowGet(), g_langStrings[6]);
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 0;
      if (UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 1, -1) != 0) {
        self->step = 14;
      }
    }
    break;
  case 7:
    if (UiTextBoxHasPrinter((UiTextBox *)((UiMsgWindow *)UiMsgWindowGet())->textBox)) {
      UiMsgWindowPrintfUtf8((UiMsgWindow *)UiMsgWindowGet(), g_langStrings[7]);
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 0;
      if (UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 1, -1) != 0) {
        self->step = 14;
      }
    }
    break;
  case 8:
    if (SysUtilIsInit()) {
      SysUtilOpenDialog(SysUtilGetCell(), 1);
      self->handler = (SysUtilSavedataHandler *)SysUtilGetDialog(SysUtilGetCell(), 1);
      SaveAutoSaveTaskRequest(self->handler, 4);
      self->step = 9;
    }
    break;
  case 9:
    if (self->handler != NULL && SysUtilSavedataIsDone()) {
      result = 0;
      if (SaveHasProfile()) {
        result = SysUtilSavedataGetResult();
        SaveProfileSetWord(SaveGetProfile(), 1, (u32)result);
      }
      SysUtilCloseDialog(SysUtilGetCell(), 1);
      if (result < 7 || result > 8) {
        self->step = 17;
      } else {
        self->step = 10;
      }
    }
    break;
  case 10:
    if (UiTextBoxHasPrinter((UiTextBox *)((UiMsgWindow *)UiMsgWindowGet())->textBox)) {
      kb = (s32)SysUtilGetSaveRequiredBytes() / 1024;
      if (kb == 0) {
        kb = 1;
      }
      sprintf(kbText, g_langStrings[2], kb);
      UiMsgWindowPrintfUtf8((UiMsgWindow *)UiMsgWindowGet(), kbText);
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 0;
      if (UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 1, -1) != 0) {
        self->step = 11;
      }
    }
    break;
  case 11:
    if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
      if (UiMsgWindowGetChoice((UiMsgWindow *)UiMsgWindowGet()) == 0) {
        self->step = 12;
      } else {
        self->step = 17;
      }
    }
    break;
  case 12:
    if (SysUtilIsInit()) {
      SysUtilOpenDialog(SysUtilGetCell(), 1);
      self->handler = (SysUtilSavedataHandler *)SysUtilGetDialog(SysUtilGetCell(), 1);
      SaveAutoSaveTaskRequest(self->handler, 2);
      self->step = 13;
    }
    break;
  case 13:
    if (self->handler != NULL && SysUtilSavedataIsDone()) {
      SysUtilCloseDialog(SysUtilGetCell(), 1);
      self->step = 1;
    }
    break;
  case 14:
    if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
      if (UiMsgWindowGetChoice((UiMsgWindow *)UiMsgWindowGet()) == 0) {
        self->step = 18;
      } else {
        self->step = self->unk21 ? 15 : 1;
      }
    }
    break;
  case 15:
    if (UiTextBoxHasPrinter((UiTextBox *)((UiMsgWindow *)UiMsgWindowGet())->textBox)) {
      UiMsgWindowPrintfUtf8((UiMsgWindow *)UiMsgWindowGet(), g_langStrings[8]);
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 0;
      if (UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 1, -1) != 0) {
        self->step = 16;
      }
    }
    break;
  case 16:
    if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
      if (UiMsgWindowGetChoice((UiMsgWindow *)UiMsgWindowGet()) == 0) {
        self->unk21 = 0;
        self->step = 1;
      } else {
        self->unk21 = 1;
        self->step = 17;
      }
    }
    break;
  case 17:
    if (UiTextBoxHasPrinter((UiTextBox *)((UiMsgWindow *)UiMsgWindowGet())->textBox)) {
      UiMsgWindowPrintfUtf8((UiMsgWindow *)UiMsgWindowGet(), g_langStrings[4]);
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 0;
      if (UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 1, -1) != 0) {
        self->step = 14;
      }
    }
    break;
  default:
    CoreTaskRemove(task, true);
    return;
  }

  if (self->fader != NULL) {
    GfxFaderStep(self->fader);
  }
  if (self->textBox == NULL) {
    return;
  }
  if (self->unk30 <= 0) {
    if (self->unk30 < 0) {
      return;
    }
    if (UiTextBoxCreatePrinter(self->textBox, 32) == 0) {
      return;
    }
    UiTextPrinterSetFont(UiTextBoxGetPrinter(self->textBox), 1);
    printer = UiTextBoxGetPrinter(self->textBox);
    printer->color[0] = g_colorWhite.x;
    printer->color[1] = g_colorWhite.y;
    printer->color[2] = g_colorWhite.z;
    printer->color[3] = g_colorWhite.w;
    printer = UiTextBoxGetPrinter(self->textBox);
    printer->outlineColor[0] = g_colorWhite.x;
    printer->outlineColor[1] = g_colorWhite.y;
    printer->outlineColor[2] = g_colorWhite.z;
    printer->outlineColor[3] = g_colorWhite.w;
    self->unk30 = self->unk30 + 1;
  } else if (self->unk30 < 2) {
    if (self->timer <= 0) {
      return;
    }
    UiTextEncodeUtf8(encoded, g_langStrings[10]);
    UiTextBoxPrint(self->textBox, 32, 240, (char *)encoded, 0, 0);
    self->textFrame = 0;
    self->unk30 = self->unk30 + 1;
  } else if (self->unk30 < 3) {
    self->textFrame = self->textFrame + 1;
    if (self->timer == 0) {
      UiTextBoxClear(self->textBox);
      self->unk30 = 1;
      return;
    }
    glyph = ((UiTextPrinter *)UiTextBoxGetPrinter(self->textBox))->glyphs;
    if (glyph == NULL) {
      return;
    }
    count = ((UiTextPrinter *)UiTextBoxGetPrinter(self->textBox))->glyphCount;
    phase = self->textFrame * 2;
    for (i = 0; i < count; i++) {
      angle = (float)phase * 3.1415927f * 0.06666667f;
      c = __builtin_cosf(angle);
      glyph->posY = c * 3.0f + 240.0f;
      phase += 8;
      glyph++;
    }
  }
}
