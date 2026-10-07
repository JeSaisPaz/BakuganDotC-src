// bdc 0x0882ccfc BtlHudRequestButtonGuide
#include "bdc.h"

/* With `show` set, starts the HUD button-guide panel (state 1, page `page`, show flag set; see
   `BtlHudUpdateButtonGuide`); otherwise only clears the show flag. Called by
   `BtlHudUpdateButtonGuide` and `BtlHudAdviceTrigger21`. */
void BtlHudRequestButtonGuide(BtlHud *self, bool show, s32 page)
{
    if (show) {
        self->guideState = 1;
        self->guidePage = page;
        self->guideShow = 1;
        return;
    }
    self->guideShow = 0;
}
