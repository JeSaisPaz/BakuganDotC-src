// bdc 0x088ce9dc UiFieldHudCtor
#include "bdc.h"

/* Constructor of the field HUD, task id 3001 (0xbb9) (base `UiScreenCtor`, vtable `0x08af2df4`).
   Object size 0xec. Initialises the embedded helper at `+0x6c` (`UiFieldHudFaderCtor`). The HUD shown over
   the field scene (task 500): guide icons `"guide_ico_05/12/13"`, adventure hints from
   `"mes_Adventure_hint_%s.bin"`, repair-point markers (`"btl_06_repairpoint01/02"`) and gauges; its
   helpers are named by the field owner with the `UiFieldHud` prefix. */

UiFieldHud *UiFieldHudCtor(UiFieldHud *self)

{
  UiScreenCtor((CoreTask *)self);
  (self->base).base.vtable = g_uiFieldHudVtbl;
  UiFieldHudFaderCtor(&self->fader);
  self->setupState[0] = '\0';
  self->setupState[1] = '\0';
  self->setupState[2] = '\0';
  self->setupState[3] = '\0';
  self->player = (ActorPlayer *)0x0;
  self->iconBase[0] = '\0';
  self->iconBase[1] = '\0';
  self->iconBase[2] = '\0';
  self->iconBase[3] = '\0';
  self->iconBase[4] = '\0';
  self->iconBase[5] = '\0';
  self->iconBase[6] = '\0';
  self->iconBase[7] = '\0';
  self->mapRect[0] = 0.0;
  self->mapRect[1] = 0.0;
  self->mapRect[2] = 0.0;
  self->mapRect[3] = 0.0;
  self->radarSize[0] = 0.0;
  self->radarSize[1] = 0.0;
  self->guideState[0] = 0xff;
  self->guideState[1] = 0xff;
  self->guideState[2] = 0xff;
  self->guideState[3] = 0xff;
  self->guideState[4] = '\0';
  self->unkA0 = 0;
  self->promptShown = '\0';
  self->promptVisible = '\0';
  self->promptTimer = 0;
  self->promptHold = 0;
  self->hintId = 0;
  self->radarHalf[0] = 0.0;
  self->radarHalf[1] = 0.0;
  self->shownPoints = 0;
  self->viewDir[0] = 0.0;
  self->viewDir[1] = 0.0;
  self->scrollPos = 0;
  self->targetCount = 0;
  self->targetsDone = 0;
  self->targetMark[0] = '\0';
  self->targetMark[1] = '\0';
  self->targetMark[2] = '\0';
  self->targetMark[3] = '\0';
  self->hintPrinter = (UiTextPrinter *)0x0;
  self->hints = (char **)0x0;
  self->blinkA = 0;
  self->promptY = -48.0;
  self->blinkB = 0;
  g_uiFieldHudEnabled = 1;
  g_uiFieldHudResetRequest = 0;
  return self;
}

