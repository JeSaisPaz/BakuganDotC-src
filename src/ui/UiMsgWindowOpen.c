// bdc 0x08816be8 UiMsgWindowOpen
#include "bdc.h"

/* Opens the message window: resets the choice (`+0x20 = -1`), pulse and pulse step (3 s at the
   current fps), optionally loads message `msgIndex` (0..30) of the current language table
   `g_langStrings` (`UiMsgWindowPrintfUtf8`), hides the 11 frame sprites and, once the text box
   has a printer, sets the text depth, colours (`g_colorWhite`) and wrap width (320), stores
   `mode` in `+0x20`, resets state `+0x0 = 0` and shows the frame sprites for the window kind
   `+0x40`: sprites 0, 1 always, 2, 3, 5, 6 only when it is 0, 8, 9 only when it is 1 (4, 7, 10 stay
   hidden). Returns 1 when opened, 0 while the printer is not ready (callers retry). */

s32 UiMsgWindowOpen(UiMsgWindow *self, s32 mode, s32 msgIndex)
{
    UiTextPrinter *printer;
    s32 fps;
    s32 opened;
    s32 i;

    self->choice = -1;
    self->pulse = 1.0f;
    opened = 0;
    fps = GfxDisplayGetFps(g_gfxDisplay);
    self->hasChoice = 0;
    self->pulseStep = -(3.0f / (float)fps);
    if (msgIndex >= 0 && msgIndex < 31) {
        UiMsgWindowPrintfUtf8(self, g_langStrings[msgIndex]);
    }
    for (i = 0; i < 11; i++) {
        self->sprites[i]->flags &= ~1u;
    }
    if (UiTextBoxGetPrinter(self->textBox) != NULL) {
        self->choice = mode;
        self->delay = -1;
        self->state = 0;
        self->closeRequested = 0;
        self->unk1d = 0;
        ((UiTextBox *)self->textBox)->packetDepth = self->depth + 1.0f;
        printer = (UiTextPrinter *)UiTextBoxGetPrinter(self->textBox);
        printer->color[0] = g_colorWhite.x;
        printer->color[1] = g_colorWhite.y;
        printer->color[2] = g_colorWhite.z;
        printer->color[3] = g_colorWhite.w;
        printer = (UiTextPrinter *)UiTextBoxGetPrinter(self->textBox);
        printer->outlineColor[0] = g_colorWhite.x;
        printer->outlineColor[1] = g_colorWhite.y;
        printer->outlineColor[2] = g_colorWhite.z;
        printer->outlineColor[3] = g_colorWhite.w;
        printer = (UiTextPrinter *)UiTextBoxGetPrinter(self->textBox);
        printer->wrapWidth = 320.0f;
        opened = 1;
        for (i = 0; i < 11; i++) {
            switch (i) {
            case 2:
            case 3:
            case 5:
            case 6:
                if (self->mode == 0) {
                    self->sprites[i]->flags |= 1;
                }
                break;
            case 8:
            case 9:
                if (self->mode == 1) {
                    self->sprites[i]->flags |= 1;
                }
                break;
            case 4:
            case 7:
            case 10:
                break;
            default:
                self->sprites[i]->flags |= 1;
                break;
            }
        }
    }
    return opened;
}
