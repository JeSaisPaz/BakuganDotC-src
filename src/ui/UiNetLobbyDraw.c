// bdc 0x089419ec UiNetLobbyDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the ad-hoc multiplayer lobby screen (task id 2000): opens a render packet at sort key 55.0, draws the highlight rect (+0x98) into it when present, then draws the text box (+0x94) when present. Draws no sprite layer of its own. */

void UiNetLobbyDraw(UiNetLobby *self)

{
  void *packet;
  
  packet = GfxNewRenderPacket(55.0f);
  if (self->highlightRect != (void *)0x0) {
    GfxRectDrawInPacket(self->highlightRect,packet);
  }
  if (self->textBox != (void *)0x0) {
    UiTextBoxDraw(self->textBox);
  }
  return;
}

