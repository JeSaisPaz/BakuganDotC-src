// bdc 0x08808718 UiCopyrightTaskStateShow
#include "bdc.h"

/* State 0 of the copyright-notice task: steps a fade sequence on `step` with the active fader
   (`GfxGetActiveFader`): wait for the text box printer and the fader, fade to black, swap the
   fader and copy its end colour into the display clear colour, print the current language's
   copyright string (g_langStrings[0], encoded with `UiTextEncodeUtf8`) in white centred at
   (240, 136), hold it for `fps * 3` frames, fade again, then switch to state 1. */

void UiCopyrightTaskStateShow(UiCopyrightTask *task)
{
  u8 text[780];
  GfxDisplay *display;
  GfxFader *fader;
  float *color;

  switch (task->step) {
  case 0:
    if (UiTextBoxCreatePrinter(task->textBox, 0x300) == 0) {
      return;
    }
    if (!GfxFaderIsFinished(GfxGetActiveFader())) {
      return;
    }
    UiTextPrinterSetFont(UiTextBoxGetPrinter(task->textBox), 0);
    task->step = 1;
    GfxFaderSetPreset(GfxGetActiveFader(), 1);
    GfxFaderStart(GfxGetActiveFader(), 10);
    return;
  case 1:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      task->step = 2;
    }
    return;
  case 2:
    if (!UiTextBoxHasPrinter(task->textBox)) {
      return;
    }
    GfxFaderSetPreset(GfxGetActiveFader(), 4);
    display = g_gfxDisplay;
    fader = GfxGetActiveFader();
    display->clearColor[0] = fader->end[0];
    display->clearColor[1] = fader->end[1];
    display->clearColor[2] = fader->end[2];
    display->clearColor[3] = fader->end[3];
    GfxFaderStart(GfxGetActiveFader(), 10);
    task->step = 3;
    return;
  case 3:
    UiTextEncodeUtf8(text, g_langStrings[0]);
    color = UiTextBoxGetColor(task->textBox);
    color[0] = g_colorWhite.x;
    color[1] = g_colorWhite.y;
    color[2] = g_colorWhite.z;
    color[3] = g_colorWhite.w;
    UiTextBoxPrint(task->textBox, 0xf0, 0x88, (char *)text, 1, 1);
    task->timer = GfxDisplayGetFps(g_gfxDisplay) * 3;
    task->step = 4;
    /* fall through */
  case 4:
    if (task->timer > 0) {
      task->timer = task->timer - 1;
      return;
    }
    GfxFaderSetPreset(GfxGetActiveFader(), 4);
    GfxFaderStart(GfxGetActiveFader(), 10);
    task->step = 5;
    return;
  default:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      task->state = 1;
      task->step = 0;
    }
    return;
  }
}
