// bdc 0x088cd178 GameDebugStageSelectCtor
#include "bdc.h"

/* Constructor of the developer stage-select menu task (task id 501 = 0x1f5, 0x38 bytes, vtable
   `g_gameDebugStageSelectVtbl`): creates three text printers (`UiTextPrinterCreate`), enables D-pad
   stick emulation, sets the clear colour to `g_colorBlack` and the fader depth, and clears the menu
   step/cursor/region/stage, highlight = 1.0 and pulse = 0. Returns `self`. */

GameDebugStageSelect *GameDebugStageSelectCtor(GameDebugStageSelect *self)
{
  GfxFader *fader;
  int i;

  CoreTaskInit(&self->base);
  self->base.vtable = &g_gameDebugStageSelectVtbl;
  g_gfxDisplay->frameSkip = 0;
  for (i = 0; i < 3; i++) {
    UiTextPrinter *printer = UiTextPrinterCreate(0);
    self->printers[i] = printer;
    printer->sjisFlag = 1;
  }
  self->pad = g_padState;
  g_padState->dpadEmulatesStick = 1;
  fader = GfxGetActiveFader();
  fader->sortKey = 20000.0f;
  g_gfxDisplay->clearColor[0] = g_colorBlack.x;
  g_gfxDisplay->clearColor[1] = g_colorBlack.y;
  g_gfxDisplay->clearColor[2] = g_colorBlack.z;
  g_gfxDisplay->clearColor[3] = g_colorBlack.w;
  self->step = 0;
  self->cursor = 0;
  memset(&self->region, 0, 8);
  self->highlight = 1.0f;
  self->pulse = 0.0f;
  return self;
}
