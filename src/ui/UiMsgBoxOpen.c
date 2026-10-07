// bdc 0x089ec27c UiMsgBoxOpen
#include "bdc.h"

/* Starts displaying a loaded message box. When the box has not finished closing
   (`UiMsgBoxIsFinished` false) it hides the choice highlight (`GfxRectSetVisible`), clears the
   choice table, sets `choice = -1`, `unk64 = 0`, the cursor fade `unk68 = 1.0` and its step
   `unk6c = -1.8 / fps` (`GfxDisplayGetFps`). Then, only if the text box has a printer
   (`UiTextBoxGetPrinter`), stores `arg` in `unk1c`, `id = 0`, `unk59 = 0`, `choices = NULL`,
   `proportional = flag`, `unk70 = 0`, the text box packet depth `unk10 + 1.0`, the colours
   `g_colorWhite` / `g_colorBlack` into `colorA` / `colorB`, `state = 1`, `shadow = 1`, `unk78 = 0`. Returns true when the printer
   exists (the box was opened), false otherwise. */

bool UiMsgBoxOpen(UiMsgBox *self, int arg, u8 flag)
{
  bool opened = false;

  if (!UiMsgBoxIsFinished(self)) {
    GfxRectSetVisible(self->highlight, 0);
    self->choices = NULL;
    self->choice = -1;
    self->unk64 = 0;
    self->unk68 = 1.0f;
    self->unk6c = -(1.8f / (float)GfxDisplayGetFps(g_gfxDisplay));
  }
  if (UiTextBoxGetPrinter(self->textBox) != NULL) {
    self->unk1c = arg;
    self->id = 0;
    self->unk59 = 0;
    self->choices = NULL;
    self->proportional = flag;
    self->unk70 = 0;
    self->textBox->packetDepth = self->unk10 + 1.0f;
    self->colorA[0] = g_colorWhite.x;
    self->colorA[1] = g_colorWhite.y;
    self->colorA[2] = g_colorWhite.z;
    self->colorA[3] = g_colorWhite.w;
    self->colorB[0] = g_colorBlack.x;
    self->colorB[1] = g_colorBlack.y;
    self->colorB[2] = g_colorBlack.z;
    self->colorB[3] = g_colorBlack.w;
    opened = true;
    self->state = 1;
    self->shadow = 1;
    self->unk78 = 0;
  }
  return opened;
}
