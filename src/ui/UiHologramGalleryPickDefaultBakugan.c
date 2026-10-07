// bdc 0x0891ba6c UiHologramGalleryPickDefaultBakugan
#include "bdc.h"

/* Returns the Bakugan id remembered in the profile (`curBakugan`, low byte). When that id is in the
   unselectable mask (`UiBakuganGetUnselectableMask`, low word), scans the first 20 entries of
   `g_uiHologramBakuganOrder` for the first Bakugan that is owned (profile bitset `ownedBakugan`)
   and not in the mask, stores it as `curBakugan` and returns it; if none qualifies the original id
   is returned unchanged. `self` is unused. */

u32 UiHologramGalleryPickDefaultBakugan(UiHologramGallery *self)
{
    u8 order[24];
    u32 mask[2];
    u32 cur;
    s32 i;

    memcpy(order, g_uiHologramBakuganOrder, 0x17);
    cur = (u32)SaveGetProfile()->data->curBakugan & 0xff;
    UiBakuganGetUnselectableMask(mask);
    if ((mask[0] & (1 << cur)) != 0) {
        for (i = 0; i < 20; i++) {
            s32 id = order[i];
            if ((u8)(SaveGetProfile()->data->ownedBakugan[id / 8] & (1 << (id % 8))) != 0 &&
                (mask[0] & (1 << id)) == 0) {
                cur = id;
                SaveGetProfile()->data->curBakugan = cur;
                break;
            }
        }
    }
    return cur;
}
