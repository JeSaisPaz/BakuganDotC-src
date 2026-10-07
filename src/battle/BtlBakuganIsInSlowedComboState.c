// bdc 0x0886aa88 BtlBakuganIsInSlowedComboState
#include "bdc.h"

/* Returns 1 when the unit is in state 0xb with flag 0x2000 of `stateFlags` set while the global
   motion time scale (`GfxGetMotionTimeScale`) is below 1.0 (slow motion; false for NaN),
   else 0. Used by the battle camera code. */
int BtlBakuganIsInSlowedComboState(BtlBakugan *self)
{
    if (self->state == 0xb && (self->stateFlags & 0x2000) != 0 &&
        GfxGetMotionTimeScale() < 1.0f) {
        return 1;
    }
    return 0;
}
