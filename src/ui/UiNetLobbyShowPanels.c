// bdc 0x08941a60 UiNetLobbyShowPanels
#include "bdc.h"

/* Runs the show animation of the two `UiNetLobbySlideAnim` panels of
   `UiNetLobby` (virtual calls, GCC 2.x vtable). With `reset`: for each panel
   rewinds its Show animation (entry 3), gives it its slide offset from `g_uiNetLobbyPanelSlots`,
   hides the active panel `+0xa0` (SetVisible, entry 5, with 0) and puts the slot's screen sprite
   at depth −128; then makes the active panel visible again and returns 1. Otherwise steps both
   Show animations and returns 1 when both have finished, 0 while either is still running. */

s32 UiNetLobbyShowPanels(UiNetLobby *self, u8 reset)
{
  UiNetLobbySlideAnim *panel;
  const VtblEntry *entry;
  s32 done;
  s32 i;

  done = 1;
  if (reset != 0) {
    for (i = 0; i < 2; i++) {
      panel = self->panels[i];
      entry = &((const VtblEntry *)panel->vtable)[3];
      ((s32 (*)(void *, u8))entry->fn)((u8 *)panel + entry->delta, 1);
      UiNetLobbySlideAnimSetOffset(self->panels[i], g_uiNetLobbyPanelSlots[i][1]);
      panel = self->panels[self->activePanel];
      entry = &((const VtblEntry *)panel->vtable)[5];
      ((void (*)(void *, u8))entry->fn)((u8 *)panel + entry->delta, 0);
      ((GfxSprite **)self->base.data)[g_uiNetLobbyPanelSlots[i][0]]->posZ = -128.0f;
    }
    panel = self->panels[self->activePanel];
    entry = &((const VtblEntry *)panel->vtable)[5];
    ((void (*)(void *, u8))entry->fn)((u8 *)panel + entry->delta, 1);
  }
  else {
    for (i = 0; i < 2; i++) {
      panel = self->panels[i];
      entry = &((const VtblEntry *)panel->vtable)[3];
      if (((s32 (*)(void *, u8))entry->fn)((u8 *)panel + entry->delta, 0) == 0) {
        done = 0;
      }
    }
  }
  return done;
}
