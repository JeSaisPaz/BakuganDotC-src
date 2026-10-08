// bdc 0x0882c55c UiTalkLayoutText
#include "bdc.h"

/* Lays out message `index` of the message table `mes` (`UiMesTableRelocate`) in the
   talk/HUD task (`UiGetTalkTask`) (id 0x6e, `BtlHudUpdate`): copies the string to `+0x6a4`,
   clears the text layer `+0x688`, measures it with font 1 in a 284-px box (`UiTextMeasure`) and
   falls back to font 0 when it needs more than two lines, then prints it (printer vslot `+0x14`)
   indented from (`x`, `y`) according to `UiTalkUsesNarrowIndent` and the line count, storing the
   position in `+0x690`/`+0x694` and the print result in `+0x918`. Returns early (nothing changed)
   when `mes` is NULL or `index` is not below the table's entry count. */

void UiTalkLayoutText(void *win, s32 index, u32 *mes, s32 x, s32 y)
{
    BtlHud *hud = win;
    UiTextPrinter *printer;
    const VtblEntry *print;
    char *text;
    s32 smallFont;
    s32 offset;
    float fx;
    float fy;

    if (mes == NULL) {
        return;
    }
    if (index >= (s32)UiMesTableRelocate(mes)) {
        return;
    }
    hud->talkPrintBusy = 0;
    hud->talkTextWidth = 0.0f;
    hud->talkTextHeight = 0.0f;
    text = hud->talkText;
    smallFont = 0;
    strcpy(text, (const char *)PspPtr(mes[index])); /* relocated table entries are pointers */
    printer = hud->overlayObj[0];
    GfxSpriteLayerClear(&printer->layer);
    printer->glyphs = NULL;
    UiTextPrinterSetFont(hud->overlayObj[0], 1);
    printer = hud->overlayObj[0];
    printer->wrapWidth = 284.0f;
    /* The listing passes +0x8c8 (height) as the 4th and +0x8c4 (width) as the 5th argument. */
    UiTextMeasure(48.0f, hud->overlayObj[0], text, &hud->talkTextHeight, &hud->talkTextWidth, NULL);
    printer = hud->overlayObj[0];
    if (!(hud->talkTextHeight <= printer->lineHeight * 2.0f)) {
        UiTextPrinterSetFont(printer, 0);
        printer = hud->overlayObj[0];
        printer->wrapWidth = 284.0f;
        UiTextMeasure(48.0f, hud->overlayObj[0], text, &hud->talkTextHeight, &hud->talkTextWidth,
                      NULL);
        smallFont = 1;
    }
    if (UiTalkUsesNarrowIndent(win) != 0) {
        x += 0x40;
    } else {
        x += 0x78;
    }
    printer = hud->overlayObj[0];
    offset = (s32)((printer->wrapWidth - hud->talkTextWidth) * 0.5f);
    if (offset >= 0) {
        x = offset + x;
    }
    if (!(hud->talkTextHeight <= printer->lineHeight * 2.0f)) {
        y += 0x14;
    } else {
        y += 0x18;
        if (hud->talkTextHeight <= printer->lineHeight) {
            y += 8;
        }
        if (smallFont != 0) {
            y += 4;
        }
    }
    fx = (float)x;
    fy = (float)y;
    print = &printer->layer.vtbl[2];
    hud->talkPrintBusy = ((u8 (*)(float, float, float, void *, char *, s32, s32, s32))print->fn)(
        fx, fy, 48.0f, (u8 *)printer + print->delta, text, 0, 0, 0);
    hud->talkTextPos[0] = fx;
    hud->talkTextPos[1] = fy;
}
