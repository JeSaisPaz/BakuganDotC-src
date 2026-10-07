// bdc 0x089ec3e4 UiMsgBoxChoiceX
#include "bdc.h"

/* Returns the screen X of choice `index` of a `UiMsgBox`: 0 without a text box or printed
   sprite. Without choices or without proportional layout it is the X of the text printer's first
   sprite (truncated to int). Otherwise it computes the target row Y = first sprite Y + printer
   `lineHeight` * `choices[index].line` (truncated), walks the printed sprites for the first one
   whose truncated Y equals it and returns its X + printer `advanceX` * `choices[index].x`
   (truncated); 0 when no sprite is on that row. */
s32 UiMsgBoxChoiceX(UiMsgBox *box, s32 index)
{
    GfxSprite *line;
    s32 targetY;

    if (box->textBox == NULL) {
        return 0;
    }
    line = ((UiTextPrinter *)UiTextBoxGetPrinter(box->textBox))->layer.head;
    if (line == NULL) {
        return 0;
    }
    if (box->choices == NULL || !box->proportional) {
        return (s32)line->posX;
    }
    targetY = (s32)(line->posY +
                    ((UiTextPrinter *)UiTextBoxGetPrinter(box->textBox))->lineHeight *
                        (float)box->choices[index].line);
    do {
        if ((s32)line->posY == targetY) {
            return (s32)(((UiTextPrinter *)UiTextBoxGetPrinter(box->textBox))->advanceX *
                             box->choices[index].x +
                         line->posX);
        }
        line = line->next;
    } while (line != NULL);
    return 0;
}
