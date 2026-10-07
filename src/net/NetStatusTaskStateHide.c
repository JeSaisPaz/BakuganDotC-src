// bdc 0x089443ec NetStatusTaskStateHide
#include "bdc.h"

/* State 4 of the netplay status overlay: step 0 advances to step 1; step 1 fades the alpha
   `alpha` out by 0.0333 per frame until it is no longer positive (then clamps it to 0 and
   advances); any other step returns to the idle state 1 (`NetStatusTaskStateIdle`).
   Every frame ends by animating the text (`NetStatusTaskAnimateText`). */

void NetStatusTaskStateHide(NetStatusTask *self)
{
    int step = self->step;

    if (step == 0) {
        self->step = step + 1;
    } else if (step == 1) {
        if (self->alpha <= 0.0f) {
            self->alpha = 0.0f;
            self->step = step + 1;
        } else {
            self->alpha = self->alpha - 0.0333f;
        }
    } else {
        self->state = 1;
        self->step = 0;
    }
    NetStatusTaskAnimateText(self);
}
