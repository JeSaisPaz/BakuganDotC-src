// bdc 0x0897c160 UiCollectionSphereClampCursorToPage
#include "bdc.h"

/* After a page change in `UiCollectionSphere`, moves the cursor back onto
   the last used cell of the new page (categories 0/1) or onto the first/last cell by direction on
   special pages. */

void UiCollectionSphereClampCursorToPage(UiCollectionSphere *self)
{
    s8 cursor;
    int base;
    int i;

    if (self->category >= 0 && self->category < 2) {
        base = self->page * 6;
        for (i = 0; i < base;) {
            int c = self->cursor - i;
            i++;
            if (c + base < (int)self->entryCount) {
                self->cursor = (s8)c;
                return;
            }
        }
        return;
    }
    cursor = 0;
    if (self->pageDir != 1) {
        cursor = 2;
        if (UiCollectionSphereGetPageKind(self, self->page) != 0) {
            cursor = 0;
        }
    }
    self->cursor = cursor;
}
