// bdc 0x0891f268 UiHologramGallerySetupMenu
#include "bdc.h"

/* Lays out the command menu of the hologram gallery screen (task 391, `maybe_UiScreen391Ctor`,
   class prefix `UiHologramGallery`; `fix_*` sprites, `DWHologramHelp` texts; main update
   `UiHologramGalleryMainPhase` with step `+0x2c`; placed holograms are save-profile bytes `+0x84 + slot`): hides
   the menu sprites (6, 7, 0xc..0x13), then shows the items available for `mode` (alpha 1, layer 8,
   button icons via `UiSetButtonIcon`) and stores `mode` in `menuMode`:
   mode 0: sprites 0xe (icon 2) and 0x11; mode 1: sprites 6 (icon 2), 0xf, 7 (icon 1), 0x10;
   mode 2: sprites 6 (icon 2), 0xf, 7 (icon 0), 0x12; other modes show nothing. */

/* sprite i of the screen's sprite table, re-read from self->base.data at every access like the asm */
#define MENU_SPRITE(i) (((UiHologramGalleryData *)self->base.data)->sprites[(i)])

void UiHologramGallerySetupMenu(UiHologramGallery *self, u8 mode)
{
    s32 i;

    MENU_SPRITE(6)->flags &= ~1u;
    MENU_SPRITE(7)->flags &= ~1u;
    MENU_SPRITE(0xc)->flags &= ~1u;
    MENU_SPRITE(0xd)->flags &= ~1u;
    MENU_SPRITE(0xe)->flags &= ~1u;
    for (i = 0xf; i < 0x14; i++) {
        MENU_SPRITE(i)->flags &= ~1u;
    }

    if (mode < 2) {
        if (mode == 0) {
            MENU_SPRITE(0xe)->flags |= 1;
            MENU_SPRITE(0xe)->alpha = 1.0f;
            MENU_SPRITE(0xe)->layerMask = 8;
            UiSetButtonIcon(MENU_SPRITE(0xe), 2);
            MENU_SPRITE(0x11)->flags |= 1;
            MENU_SPRITE(0x11)->alpha = 1.0f;
            MENU_SPRITE(0x11)->layerMask = 8;
        } else {
            MENU_SPRITE(6)->flags |= 1;
            MENU_SPRITE(6)->alpha = 1.0f;
            MENU_SPRITE(6)->layerMask = 8;
            UiSetButtonIcon(MENU_SPRITE(6), 2);
            MENU_SPRITE(0xf)->flags |= 1;
            MENU_SPRITE(0xf)->alpha = 1.0f;
            MENU_SPRITE(0xf)->layerMask = 8;
            MENU_SPRITE(7)->flags |= 1;
            MENU_SPRITE(7)->alpha = 1.0f;
            MENU_SPRITE(7)->layerMask = 8;
            UiSetButtonIcon(MENU_SPRITE(7), 1);
            MENU_SPRITE(0x10)->flags |= 1;
            MENU_SPRITE(0x10)->alpha = 1.0f;
            MENU_SPRITE(0x10)->layerMask = 8;
        }
    } else if (mode < 3) {
        MENU_SPRITE(6)->flags |= 1;
        MENU_SPRITE(6)->alpha = 1.0f;
        MENU_SPRITE(6)->layerMask = 8;
        UiSetButtonIcon(MENU_SPRITE(6), 2);
        MENU_SPRITE(0xf)->flags |= 1;
        MENU_SPRITE(0xf)->alpha = 1.0f;
        MENU_SPRITE(0xf)->layerMask = 8;
        MENU_SPRITE(7)->flags |= 1;
        MENU_SPRITE(7)->alpha = 1.0f;
        MENU_SPRITE(7)->layerMask = 8;
        UiSetButtonIcon(MENU_SPRITE(7), 0);
        MENU_SPRITE(0x12)->flags |= 1;
        MENU_SPRITE(0x12)->alpha = 1.0f;
        MENU_SPRITE(0x12)->layerMask = 8;
    }
    self->menuMode = (s8)mode;
}

#undef MENU_SPRITE
