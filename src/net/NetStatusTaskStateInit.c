// bdc 0x08943ea0 NetStatusTaskStateInit
#include "bdc.h"

/* State 0 of the netplay status overlay, run in steps:
   step 0: if the overlay has a text box, creates its printer (`UiTextBoxCreatePrinter`,
           0x80 chars), selects font 1 and copies green `g_colorGreen` into the printer's text and
           outline colours, then advances; without a box (or while the printer can't be created yet)
           it does nothing and retries next frame.
   step 1: prints the requested message (`g_netStatusMessage` → `g_netStatusMessageStrings` →
           `g_langStrings`) centred at (240, 136), takes over the printer's glyph sprites, sets
           flag 0x20 and clears flag 0x1 on each, resets frame and alpha and advances.
   any other step (incl. negative): switches to state 1, step 0. */

void NetStatusTaskStateInit(NetStatusTask *self)

{
  UiTextPrinter *printer;
  GfxSprite *glyph;
  int count;
  int i;
  u8 text[136];

  if (self->step == 0) {
    if (self->box == NULL) {
      return;
    }
    if (UiTextBoxCreatePrinter(self->box, 0x80) == 0) {
      return;
    }
    UiTextPrinterSetFont((UiTextPrinter *)UiTextBoxGetPrinter(self->box), 1);
    printer = (UiTextPrinter *)UiTextBoxGetPrinter(self->box);
    printer->color[0] = g_colorGreen.x;
    printer->color[1] = g_colorGreen.y;
    printer->color[2] = g_colorGreen.z;
    printer->color[3] = g_colorGreen.w;
    printer = (UiTextPrinter *)UiTextBoxGetPrinter(self->box);
    printer->outlineColor[0] = g_colorGreen.x;
    printer->outlineColor[1] = g_colorGreen.y;
    printer->outlineColor[2] = g_colorGreen.z;
    printer->outlineColor[3] = g_colorGreen.w;
    self->step = self->step + 1;
    return;
  }
  if (self->step == 1) {
    self->message = g_netStatusMessage;
    UiTextEncodeUtf8(text, g_langStrings[g_netStatusMessageStrings[g_netStatusMessage]]);
    UiTextBoxPrint(self->box, 240, 136, (char *)text, 1, 0);
    glyph = ((UiTextPrinter *)UiTextBoxGetPrinter(self->box))->glyphs;
    count = ((UiTextPrinter *)UiTextBoxGetPrinter(self->box))->glyphCount;
    self->glyphs = glyph;
    self->glyphCount = count;
    for (i = 0; i < count; i++) {
      glyph->flags |= 0x20;
      glyph->flags &= ~1u;
      glyph++;
    }
    self->frame = 0;
    self->alpha = 0.0f;
    self->step = self->step + 1;
    return;
  }
  self->state = 1;
  self->step = 0;
}
