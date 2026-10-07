// bdc 0x0894423c NetStatusTaskStateShow
#include "bdc.h"

/* State 2 of the netplay status overlay. Step 0: if the requested message
   `g_netStatusMessage` differs from `message`, stores it and (with a text box) prints its
   language string (`g_netStatusMessageStrings` -> `g_langStrings`, encoded with
   `UiTextEncodeUtf8`) centred at (240, 136), takes the printer's glyph sprites and sets their
   fade flag 0x20 while clearing the visible bit 0; then resets `frame` and `alpha` to 0 and moves
   to step 1. Step 1 fades `alpha` in by 0.0333 per frame until it is no longer below 1, then
   clamps it to 1 and moves to step 2. Step 2 (or a negative step) switches to state 3 with step 0.
   Every frame ends by animating the text (`NetStatusTaskAnimateText`). */

void NetStatusTaskStateShow(NetStatusTask *self)
{
    UiTextPrinter *printer;
    GfxSprite *glyph;
    int count;
    int i;
    u8 text[128];

    if (self->step == 0) {
        if (self->message != g_netStatusMessage) {
            self->message = g_netStatusMessage;
            if (self->box != NULL) {
                UiTextEncodeUtf8(text, g_langStrings[g_netStatusMessageStrings[self->message]]);
                UiTextBoxPrint(self->box, 240, 136, (char *)text, 1, 0);
                printer = (UiTextPrinter *)UiTextBoxGetPrinter(self->box);
                glyph = printer->glyphs;
                printer = (UiTextPrinter *)UiTextBoxGetPrinter(self->box);
                count = printer->glyphCount;
                self->glyphs = glyph;
                self->glyphCount = count;
                for (i = 0; i < count; i++, glyph++) {
                    glyph->flags |= 0x20;
                    glyph->flags &= ~1u;
                }
            }
        }
        self->frame = 0;
        self->alpha = 0.0f;
        self->step++;
    } else if (self->step == 1) {
        if (self->alpha < 1.0f) {
            self->alpha = self->alpha + 0.0333f;
        } else {
            self->alpha = 1.0f;
            self->step++;
        }
    } else {
        self->state = 3;
        self->step = 0;
    }
    NetStatusTaskAnimateText(self);
}
