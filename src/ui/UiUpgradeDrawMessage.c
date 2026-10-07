// bdc 0x08912bc4 UiUpgradeDrawMessage
#include "bdc.h"

/* Draws the message text layer `+0x13e4` of the Bakugan upgrade screen (`UiUpgradeCtor`, task
   490; selected Bakugan `+0x16a8`, selected slot `+0x1698`): sets its outline colour from
   `g_colorWhite`, draws it in a render packet at depth 4000 (`GfxSpriteLayerDraw`) and
   applies the text alpha `textColor[3]` to every glyph sprite (`+0xbc`). */

void UiUpgradeDrawMessage(UiUpgrade *self)
{
    UiTextPrinter *printer = self->textPrinter;

    if (printer != NULL) {
        GfxSprite *glyph;
        int i;

        printer->outlineColor[0] = g_colorWhite.x;
        printer->outlineColor[1] = g_colorWhite.y;
        printer->outlineColor[2] = g_colorWhite.z;
        printer->outlineColor[3] = g_colorWhite.w;
        printer = self->textPrinter;
        GfxSpriteLayerDraw(&printer->layer, GfxNewRenderPacket(4000.0f));
        i = 0;
        if (self->textPrinter->glyphCount > 0) {
            glyph = self->textPrinter->glyphs;
            for (;;) {
                glyph->alpha = self->textColor[3];
                i++;
                if (self->textPrinter->glyphCount <= i) {
                    break;
                }
                glyph = glyph->next;
            }
        }
    }
}
