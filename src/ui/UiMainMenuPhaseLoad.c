// bdc 0x089a6d0c UiMainMenuPhaseLoad
#include "bdc.h"

/* Phase 0 of `UiMainMenu`: step 0 builds the menu (`UiMainMenuCreateSprites`, the 3D menu
   models `UiMainMenuCreateModels` — `menu_daiza.gmo`, `menu_worldmap.gmo`, `menu_itembox.gmo`,
   `menu_gauntlet.gmo`, `menu_credit.gmo` — and the line/sea material hooks and dimmed item-box
   lights); step 1 starts `main_bg.fab` in shared-anim slot 0 (`UiSharedAnimStart`, depth 10);
   any other step (2, or negative) starts a black fade-in (8 frames), queues BGM track 0x17 when
   `UiMainMenuShouldStartBgm` says so, and advances to phase 1 with step 0. */

void UiMainMenuPhaseLoad(UiMainMenu *self)
{
    GfxFader *fader;

    switch (self->base.phaseStep) {
    case 0:
        UiMainMenuCreateSprites(self);
        UiMainMenuCreateModels(self);
        UiMainMenuHookLineMaterial(self);
        UiMainMenuHookSeaMaterial(self);
        UiMainMenuDimItemBoxLights(self);
        self->base.phaseStep++;
        return;
    case 1:
        UiSharedAnimStart(10.0f, 0.0f, 0.0f, self, (void *)"main_bg.fab", 0, 0);
        self->base.phaseStep++;
        return;
    default:
        break;
    }
    fader = GfxGetActiveFader();
    fader->start[0] = 0.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 1.0f;
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 0.0f;
    GfxFaderStart(GfxGetActiveFader(), 8);
    if (UiMainMenuShouldStartBgm() != 0) {
        SndBgmQueuePlay(0, 0x17, 1, 0);
    }
    self->base.phaseStep = 0;
    self->base.phase++;
}
