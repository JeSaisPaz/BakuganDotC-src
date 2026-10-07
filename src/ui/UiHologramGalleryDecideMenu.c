// bdc 0x08920d0c UiHologramGalleryDecideMenu
#include "bdc.h"

/* Handles the confirm button of the hologram gallery screen (task 391, `UiHologramGalleryCtor`;
   step 8 of `UiHologramGalleryMainPhase`, step `+0x2c`), by the active panel `+0x74`:
   - panel 0 (command menu), by `menuCursor`: 0 → step 4; 1 → clears `slot` and `listMode`, step 9;
     2 → if save word 0x2d is non-zero arms message (7, 4, 3) and goes to step 0x22, else step 4;
   - panel 1 (slot list): in list mode sets `exitMode = 1`, restarts the list pulse off and goes to
     step 0xe; otherwise, if the selected slot holds no hologram (save-profile `placedHolograms`),
     clears `helpCursor`/`helpPage` and goes to step 0xe, else fetches the slot cost of the placed
     hologram (variant `(id - 14) % 3`, attribute mapped from `(id - 14) / 3`) into the
     `bonusRecord..slotPoints` record, arms message (10, 0x10, 0xd) and goes to step 0x22;
   - panel 2 → step 0x1d; any other panel: nothing. */

void UiHologramGalleryDecideMenu(UiHologramGallery *self)
{
    HologramSlotCost cost;
    s8 panel;
    s8 item;
    u8 variant;
    int attr;
    int id;

    panel = self->panel;
    if (panel < 1) {
        if (panel < 0) {
            return;
        }
        item = self->menuCursor;
        if (item < 1) {
            if (item >= 0) {
                self->base.phaseStep = 4;
            }
        } else if (item < 2) {
            self->slot = 0;
            self->listMode[0] = 0;
            self->base.phaseStep = 9;
        } else if (item < 3) {
            if (SaveProfileGetWord(SaveGetProfile(), 0x2d) != 0) {
                UiHologramGalleryArmMessage(self, 7, 4, 3);
                self->base.phaseStep = 0x22;
            } else {
                self->base.phaseStep = 4;
            }
        }
    } else if (panel < 2) {
        if (self->listMode[0] == 1) {
            self->exitMode = 1;
            UiHologramGalleryStartListPulse(self, 0);
            self->base.phaseStep = 0xe;
        } else if (SaveGetProfile()->data->placedHolograms[(u8)self->slot] == 0) {
            self->helpCursor = 0;
            self->helpPage = 0;
            self->base.phaseStep = 0xe;
        } else {
            id = SaveGetProfile()->data->placedHolograms[(u8)self->slot];
            variant = (u8)((id - 0xe) % 3);
            id = SaveGetProfile()->data->placedHolograms[(u8)self->slot];
            attr = UiHologramGalleryMapAttribute(true, (u8)((id - 0xe) / 3));
            UiHologramGalleryGetSlotCost(&cost, self, variant, (u8)attr);
            /* +0x2254..+0x2263: bonusRecord, bonusAmount, slotOldPrice, slotPoints */
            *(HologramSlotCost *)self->bonusRecord = cost;
            UiHologramGalleryArmMessage(self, 10, 0x10, 0xd);
            self->base.phaseStep = 0x22;
        }
    } else if (panel < 3) {
        self->base.phaseStep = 0x1d;
    }
}
