// bdc 0x0898e36c UiCollectionFigureCheckConfirm
#include "bdc.h"

/* Checks the confirm button (pad pressed `0x4000`) in
   `UiCollectionFigure`: returns 0 if not pressed, 1 if the selected cell
   holds an entry (`entryIds[page * 6 + cursor]` non-zero), 2 if it is empty (the main phase plays
   the error sound). */

int UiCollectionFigureCheckConfirm(UiCollectionFigure *self)
{
    if ((self->base.pad->pressed & 0x4000) != 0) {
        return self->entryIds[self->cursor + self->page * 6] != 0 ? 1 : 2;
    }
    return 0;
}
