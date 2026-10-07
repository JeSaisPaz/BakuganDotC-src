// bdc 0x08941bec UiNetLobbyHidePanels
#include "bdc.h"

/* Runs the hide animation of the two `UiNetLobbySlideAnim` panels of
   `UiNetLobby` (virtual calls, GCC 2.x vtable): `reset` rewinds both Hide
   animations (entry 4) and returns 1; otherwise steps them, hiding each panel's sprite
   (SetVisible, entry 5, with 0) when its animation is done, and returns 1 when both have
   finished, 0 while either is still running. */

s32 UiNetLobbyHidePanels(UiNetLobby *self, u8 reset)
{
  UiNetLobbySlideAnim *panel;
  const VtblEntry *entry;
  s32 done;
  s32 i;

  done = 1;
  if (reset != 0) {
    for (i = 0; i < 2; i++) {
      panel = self->panels[i];
      entry = &((const VtblEntry *)panel->vtable)[4];
      ((s32 (*)(void *, u8))entry->fn)((u8 *)panel + entry->delta, 1);
    }
  }
  else {
    for (i = 0; i < 2; i++) {
      panel = self->panels[i];
      entry = &((const VtblEntry *)panel->vtable)[4];
      if (((s32 (*)(void *, u8))entry->fn)((u8 *)panel + entry->delta, 0) == 0) {
        done = 0;
      }
      else {
        panel = self->panels[i];
        entry = &((const VtblEntry *)panel->vtable)[5];
        ((void (*)(void *, u8))entry->fn)((u8 *)panel + entry->delta, 0);
      }
    }
  }
  return done;
}
