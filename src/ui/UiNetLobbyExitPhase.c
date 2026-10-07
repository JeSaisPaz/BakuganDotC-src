// bdc 0x08941ecc UiNetLobbyExitPhase
#include "bdc.h"

/* Phase 4 of `UiNetLobby`: sets the menu result and closes the screen: 0 (and
   `g_uiKeepSharedBg` = 1, keep the shared background) when `+0x88` is set, otherwise from the
   choice `+0x30`: 0 → 1, 1 → 2, anything else → 0. */
void UiNetLobbyExitPhase(UiNetLobby *self)
{
    s32 result = 0;

    if (self->leaving != 0) {
        g_uiKeepSharedBg = 1;
    } else {
        s32 choice = (s32)self->base.unk30;
        if (choice > 0) {
            if (choice < 2) {
                result = 2;
            }
        } else if (choice >= 0) {
            result = 1;
        }
    }
    UiSetMenuResult(&self->base, result);
    self->base.closeRequested = 1;
}
