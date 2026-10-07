// bdc 0x088eeb10 GameEventCheckSkip
#include "bdc.h"

/* While a skippable wait runs (`+0x272` set, wait type 1 or 2), pressing pad button 8 (START)
   enables skip mode (`+0x273` bit 0), fades the screen to black in 5 frames (unless `+0x272 == 3`)
   and switches to state 4 (run the rest of the script at once). */

void GameEventCheckSkip(GameEvent *self) {
    GfxFader *fader;

    if (self->blocking != 0 && (self->waitType == 1 || self->waitType == 2) && g_padState != NULL &&
        (g_padState->pressed & 8) != 0) {
        self->flags = self->flags | 1;
        if (self->blocking != 3) {
            fader = GfxGetActiveFader();
            fader->start[0] = 0.0f;
            fader->start[1] = 0.0f;
            fader->start[2] = 0.0f;
            fader->start[3] = 0.0f;
            fader = GfxGetActiveFader();
            fader->end[0] = 0.0f;
            fader->end[1] = 0.0f;
            fader->end[2] = 0.0f;
            fader->end[3] = 1.0f;
            fader = GfxGetActiveFader();
            GfxFaderStart(fader, 5);
        }
        self->runStep = 0;
        self->state = 4;
    }
}
