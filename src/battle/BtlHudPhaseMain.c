// bdc 0x0883a3cc BtlHudPhaseMain
#include "bdc.h"

/* HUD phase 2 handler (the in-battle HUD, `BtlHudUpdate`): when UI window 0 is active, either
   advances to the next phase (clearing `phaseStep`) when window 2 reports active (== 1), or runs the
   HUD widget updaters in their fixed order. Does nothing while window 0 is inactive. */

void BtlHudPhaseMain(BtlHud *self)
{
    if (UiGetWindowActive(0) == 0) {
        return;
    }
    if (UiGetWindowActive(2) == 1) {
        self->phaseStep = 0;
        self->phase = self->phase + 1;
        return;
    }
    BtlHudUpdatePlayerGauges(self);
    BtlHudUpdateTargetMarker(self);
    BtlHudUpdateTimer(self);
    BtlHudUpdateComboCounter(self);
    BtlHudUpdateScriptMessage(self);
    BtlHudUpdateStageIntro(self);
    BtlHudUpdateChargeGauge(self);
    BtlHudUpdateThreatMarkers(self);
    BtlHudUpdateCardCutIns(self);
    BtlHudUpdateNamePlates(self);
    BtlHudUpdateScoreBoard(self);
    BtlHudUpdateStatusCountdown(self);
    BtlHudUpdateRadar(self);
    UiTalkWindowUpdate(self);
    BtlHudUpdateControlHints(self);
    BtlHudUpdateButtonGuide(self);
    BtlHudUpdateMultiRoundBanner(self);
    BtlHudUpdateRoundWinLamps(self);
    BtlHudUpdateAdvice(self);
    BtlHudUpdateArtReadyFlash(self);
}
