// bdc 0x08840104 BtlHudPhaseResultSelect
#include "bdc.h"

/* HUD phase 4 (`BtlHudUpdate` table `0x08a64b9c`): moves to phase 8 when UI window 7 is active,
   5/6/7 for windows 4/5/6, otherwise keeps the finish banner running
   (`BtlHudUpdateBattleEndBanner`). */
void BtlHudPhaseResultSelect(BtlHud *self)
{
    if (UiGetWindowActive(7) == 1) {
        self->phase = 8;
        self->phaseStep = 0;
    } else if (UiGetWindowActive(4) == 1) {
        self->phase = 5;
        self->phaseStep = 0;
    } else if (UiGetWindowActive(5) == 1) {
        self->phase = 6;
        self->phaseStep = 0;
    } else if (UiGetWindowActive(6) == 1) {
        self->phase = 7;
        self->phaseStep = 0;
    } else {
        BtlHudUpdateBattleEndBanner(self);
    }
}
