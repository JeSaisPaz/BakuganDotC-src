// bdc 0x0891d144 UiHologramGalleryGetSlotCost
#include "bdc.h"

/* Fills `out` (a HologramSlotCost) for placing hologram (`attr`, `variant`) in the selected slot
   (`slot`) of the hologram gallery screen (`UiHologramGalleryCtor`, task 391): the slot's
   current hologram (profile `placedHolograms[slot]`, decoded as `(id - 0xe)` → variant `% 3`,
   attribute `/ 3` via `UiHologramGalleryMapAttribute`; 0xff/0xff when empty), the new and old
   prices from `g_hologramParams` (old price 0 when empty) and the available points (profile
   `points` minus word 0x2d). */

void UiHologramGalleryGetSlotCost(void *out, UiHologramGallery *screen, u32 variant, u32 attr)
{
    HologramSlotCost *cost = out;
    u32 placed;
    u32 oldVariant;
    u32 oldAttr;
    s32 newPrice;
    s32 oldPrice;
    s32 points;
    u32 spent;

    placed = SaveGetProfile()->data->placedHolograms[(u8)screen->slot];
    if (placed == 0) {
        oldVariant = 0xff;
        oldAttr = 0xff;
    } else {
        oldVariant = (u8)((s32)(placed - 0xe) % 3);
        oldAttr = (u8)UiHologramGalleryMapAttribute(true, (u8)((s32)(placed - 0xe) / 3));
    }
    newPrice = g_hologramParams[(attr & 0xff) * 3 + (variant & 0xff)].price;
    if (placed == 0) {
        oldPrice = 0;
    } else {
        oldPrice = g_hologramParams[oldAttr * 3 + oldVariant].price;
    }
    points = SaveGetProfile()->data->points;
    spent = SaveProfileGetWord(SaveGetProfile(), 0x2d);
    cost->oldVariant = (u8)oldVariant;
    cost->oldAttribute = (u8)oldAttr;
    cost->newPrice = newPrice;
    cost->oldPrice = oldPrice;
    cost->points = points - (s32)spent;
}
