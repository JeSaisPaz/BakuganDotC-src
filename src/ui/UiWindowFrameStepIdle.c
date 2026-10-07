// bdc 0x089ff078 UiWindowFrameStepIdle
#include "bdc.h"

/* Empty state-0 (idle) step of a 9-slice window frame (`UiWindowFrame`, vtable `0x08af5954`): entry
   of the state table `0x08ac61f0` before UiWindowFrameStepOpen (state 1) and
   UiWindowFrameStepClose (state 2); just `jr ra`. */
void UiWindowFrameStepIdle(UiWindowFrame *self)
{
    (void)self;
}
