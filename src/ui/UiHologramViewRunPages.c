// bdc 0x08929fb4 UiHologramViewRunPages
#include "bdc.h"

/* Page sequencer of the hologram detail view (`UiHologramViewCtor`, task 392) on step `+0x708`:
   sets up the tip sprites and fades the screen in, shows page `pageIds[tipPage]` (help text and
   voice via `UiHologramViewSetHelpText`, text fade in), waits for button 0x4000, plays sound 0
   and stops the voice (`UiHologramViewStopVoice`), fades the text and the screen out; returns
   0 while running and 1 once the step has reached 9. */

int UiHologramViewRunPages(UiHologramView *self)
{
    switch (self->step) {
    case 0:
        UiHologramViewInitTipSprites(self);
        UiHologramViewStartFade(self, 0);
        self->step++;
        break;
    case 1:
        if (UiHologramViewUpdateFade(self, 0) == 1) {
            self->step = 2;
        }
        break;
    case 2:
        self->page = self->pageIds[self->tipPage];
        UiHologramViewSetHelpText(self, 0, 0x14);
        self->step++;
        break;
    case 3:
        if (UiHologramViewFadeText(self, 0) == 1) {
            self->step = 4;
        }
        break;
    case 4:
        if ((self->base.pad->pressed & 0x4000) != 0) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0, 0, 0);
            }
            UiHologramViewStopVoice();
            self->step = 5;
        }
        break;
    case 5:
        UiHologramViewSetHelpText(self, 1, 0x14);
        self->step++;
        break;
    case 6:
        if (UiHologramViewFadeText(self, 1) == 1) {
            self->step = 7;
        }
        break;
    case 7:
        UiHologramViewStartFade(self, 1);
        self->step++;
        break;
    case 8:
        if (UiHologramViewUpdateFade(self, 1) == 1) {
            self->step = 9;
        }
        break;
    default:
        return 1;
    }
    return 0;
}
