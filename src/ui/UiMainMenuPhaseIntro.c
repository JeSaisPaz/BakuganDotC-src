// bdc 0x089a7220 UiMainMenuPhaseIntro
#include "bdc.h"

/* Phase 1 of `UiMainMenu`: waits for the fade-in, plays `main_start.fab`, then
   sets up the menu cursor (`UiMainMenuInitItemSprites`, `UiMainMenuCacheModelHeights`), marks
   the textures `main_bg00`, `line`, `f0_z_sea01`, `worldmap` (`GfxTextureMoveToVram`) and
   advances to phase 2 (main). */

void UiMainMenuPhaseIntro(UiMainMenu *self)

{
  int step;

  step = self->base.phaseStep;
  if (step < 1) {
    if (-1 < step) {
      if (!GfxFaderIsFinished(GfxGetActiveFader())) {
        return;
      }
      (self->base).phaseStep = (self->base).phaseStep + 1;
      return;
    }
  }
  else if (step < 2) {
    UiSharedAnimStart(100.0,0.0,0.0,self,"main_start.fab",1,'\0');
    (self->base).phaseStep = (self->base).phaseStep + 1;
    return;
  }
  UiMainMenuInitItemSprites(self,'\0');
  UiMainMenuCacheModelHeights(self);
  (self->base).phaseStep = 0;
  (self->base).phase = (self->base).phase + 1;
  GfxTextureMoveToVram("main_bg00");
  GfxTextureMoveToVram("line");
  GfxTextureMoveToVram("f0_z_sea01");
  GfxTextureMoveToVram("worldmap");
  return;
}

