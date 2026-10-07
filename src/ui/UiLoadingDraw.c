// bdc 0x0890bb2c UiLoadingDraw
#include "bdc.h"

/* Draw (vtable slot `+0x24`) of the now-loading screen (task 10100 / 0x2774, 0x240 bytes, vtable
   `0x08af47dc`, `UiLoadingCtor`; shared UI objects `0x08ac0e80`): in states 2..4 advances the
   shared animation (`GfxFabListDraw`), opens a render packet at depth 1610, draws the shared sprite
   layer and the message box (`UiTextBoxDraw`) and, for the propeller themes
   (`UiLoadingIsPropellerTheme`), `UiLoadingDrawPropellers`. */

void UiLoadingDraw(UiLoading *self)
{
  void *packet;
  bool active = false;

  if (self->state >= 2 && self->state < 5) {
    active = true;
  }
  if (active) {
    if (g_uiLoadingShared->fab != NULL) {
      GfxFabListDraw((void **)&g_uiLoadingShared->fabList);
    }
    packet = GfxNewRenderPacket(1610.0f);
    GfxSpriteLayerDraw(g_uiLoadingShared->layer, packet);
    if (g_uiLoadingShared->box != NULL && UiTextBoxHasPrinter(g_uiLoadingShared->box)) {
      UiTextBoxDraw(g_uiLoadingShared->box);
    }
    if (UiLoadingIsPropellerTheme(self)) {
      UiLoadingDrawPropellers(self);
    }
  }
}
