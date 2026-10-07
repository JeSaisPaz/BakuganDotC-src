// bdc 0x089206c8 UiHologramGalleryMoveCursor
#include "bdc.h"

/* Moves the cursor of the active panel `+0x74` of the hologram gallery screen
   (`UiHologramGalleryCtor`, task 391) on the pad repeat bits: panel 0 wraps `menuCursor` over
   the 3 menu items on UP (0x10) / DOWN (0x40); panel 1 steps `slot` over the `slotCount` slots on
   UP/DOWN, passing through the list-mode row (`listMode`) between the last and first slot;
   panel 2 wraps `helpCursor` over the 6 help entries on LEFT (0x80) / RIGHT (0x20). Returns 1 when
   one of those buttons was handled, 0 otherwise (no button, or a panel outside 0..2). */

int UiHologramGalleryMoveCursor(UiHologramGallery *self)
{
    s8 panel;
    PadState *pad;

    panel = self->panel;
    if (panel <= 0) {
        if (panel < 0) {
            return 0;
        }
        pad = self->base.pad;
        if ((pad->repeat & 0x10) != 0) {
            self->menuCursor = (self->menuCursor == 0) ? 2 : (s8)(self->menuCursor - 1);
            return 1;
        }
        if ((pad->repeat & 0x40) != 0) {
            self->menuCursor = (self->menuCursor == 2) ? 0 : (s8)(self->menuCursor + 1);
            return 1;
        }
        return 0;
    }
    if (panel < 2) {
        pad = self->base.pad;
        if ((pad->repeat & 0x10) != 0) {
            if (self->listMode[0] != 0) {
                self->listMode[0] = 0;
                self->slot = self->slotCount - 1;
            } else if (self->slot == 0) {
                self->listMode[0] = 1;
            } else {
                self->slot = self->slot - 1;
            }
            return 1;
        }
        if ((pad->repeat & 0x40) != 0) {
            if (self->listMode[0] != 0) {
                self->listMode[0] = 0;
                self->slot = 0;
            } else if (self->slot == self->slotCount - 1) {
                self->listMode[0] = 1;
            } else {
                self->slot = self->slot + 1;
            }
            return 1;
        }
        return 0;
    }
    if (panel < 3) {
        pad = self->base.pad;
        if ((pad->repeat & 0x80) != 0) {
            self->helpCursor = (self->helpCursor == 0) ? 5 : (s8)(self->helpCursor - 1);
            return 1;
        }
        if ((pad->repeat & 0x20) != 0) {
            self->helpCursor = (self->helpCursor == 5) ? 0 : (s8)(self->helpCursor + 1);
            return 1;
        }
    }
    return 0;
}
