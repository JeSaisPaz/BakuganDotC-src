// bdc 0x089209d0 UiHologramGalleryFlashSelection
#include "bdc.h"

/* Starts the decide flash (`UiFlashStart`, 4 frames) on the selected item's sprites of the active
   panel of the hologram gallery screen (`UiHologramGalleryCtor`, task 391): panel 0 flashes the
   menu item pair (sprites 180+/183+ `menuCursor`) or, past item 1, sprite 128; panel 1 flashes the
   board slot sprite 103+`slot` (sprite 128 when `listMode` is set); panel 2 flashes the help item
   pair (sprites 134+/141+ `helpCursor`). Other panels do nothing. */

void UiHologramGalleryFlashSelection(UiHologramGallery *self)
{
    s8 panel = self->panel;
    UiHologramGalleryData *data;

    if (panel <= 0) {
        if (panel < 0) {
            return;
        }
        data = (UiHologramGalleryData *)self->base.data;
        if (self->menuCursor < 2) {
            UiFlashStart(4.0f, data->sprites[180 + self->menuCursor], 0, 0);
            UiFlashStart(4.0f, ((UiHologramGalleryData *)self->base.data)->sprites[183 + self->menuCursor], 0, 1);
        } else {
            UiFlashStart(4.0f, data->sprites[128], 0, 0);
        }
    } else if (panel < 2) {
        data = (UiHologramGalleryData *)self->base.data;
        if ((s8)self->listMode[0] == 0) {
            UiFlashStart(4.0f, data->sprites[103 + self->slot], 0, 0);
        } else {
            UiFlashStart(4.0f, data->sprites[128], 0, 0);
        }
    } else if (panel < 3) {
        UiFlashStart(4.0f, ((UiHologramGalleryData *)self->base.data)->sprites[134 + self->helpCursor], 0, 0);
        UiFlashStart(4.0f, ((UiHologramGalleryData *)self->base.data)->sprites[141 + self->helpCursor], 0, 1);
    }
}
