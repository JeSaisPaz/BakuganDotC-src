// bdc 0x08989bc0 UiCollectionTheaterChangePage
#include "bdc.h"

/* Changes the target page `targetPage` of `UiCollectionTheater` when
   Left/Right is pressed at the grid edge (pages 0..3, no wrap), recording the direction in `pageDir`
   (1 left, 2 right). Returns 1 when the page changes. */

int UiCollectionTheaterChangePage(UiScreen *screen)
{
    UiCollectionTheater *self = (UiCollectionTheater *)screen;
    s8 page = self->page;
    PadState *pad = screen->pad;

    self->targetPage = page;
    if (((s8)pad->repeat & 0x80) != 0) {
        if (self->cursor % 3 == 0 && self->targetPage != 0) {
            self->targetPage = self->targetPage - 1;
            self->pageDir = 1;
            return 1;
        }
    } else if ((pad->repeat & 0x20) != 0 && page != 3 && self->cursor % 3 == 2) {
        s8 t = self->targetPage;
        self->pageDir = 2;
        self->targetPage = t + 1;
        return 1;
    }
    return 0;
}
