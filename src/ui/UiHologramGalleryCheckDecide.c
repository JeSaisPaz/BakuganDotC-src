// bdc 0x0892087c UiHologramGalleryCheckDecide
#include "bdc.h"

/* On CROSS (pad `pressed` bit 0x4000) returns the decision for the active panel `+0x74` of the
   hologram gallery screen (`UiHologramGalleryCtor`, task 391): panel 0 (menu) returns 2 when the
   menu item under `menuCursor` is disabled (`disabledItems` bit), else 1; panel 1 (slot list)
   returns 1 on a slot row, and on the list-mode row 1 when `menuMode` is 1, else 2; panel 2 (help
   list) fetches the slot cost of the selected hologram (`UiHologramGalleryGetSlotCost`) into
   `+0x2254..+0x2263` and returns 2 when the points do not cover the new price, else 1. Returns 0
   without CROSS or on any other panel. */

int UiHologramGalleryCheckDecide(UiHologramGallery *self)
{
    HologramSlotCost cost;
    s8 panel;

    if ((self->base.pad->pressed & 0x4000) == 0) {
        return 0;
    }
    panel = self->panel;
    if (panel <= 0) {
        if (panel < 0) {
            return 0;
        }
        if ((self->disabledItems & (1 << self->menuCursor)) != 0) {
            return 2;
        }
        return 1;
    }
    if (panel < 2) {
        if (self->listMode[0] == 0) {
            return 1;
        }
        if (self->menuMode == 1) {
            return 1;
        }
        return 2;
    }
    if (panel < 3) {
        UiHologramGalleryGetSlotCost(&cost, self, (u8)self->helpPage,
                                     self->helpOrder[self->helpCursor]);
        /* +0x2254..+0x2263: bonusRecord, bonusAmount, slotOldPrice, slotPoints */
        *(HologramSlotCost *)self->bonusRecord = cost;
        if (cost.points < cost.newPrice) {
            return 2;
        }
        return 1;
    }
    return 0;
}
