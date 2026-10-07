// bdc 0x089ebfec UiMsgBoxDraw
#include "bdc.h"

/* Draws the message box frame: calls its owner's draw method (vtable `+0x1c`) at depth `+0x10` and
   draws the choice highlight rect (`+0x7c`, `GfxRectDrawInPacket`) into a render packet just
   above it. */

void UiMsgBoxDraw(UiMsgBox *self)
{
  UiWindowFrame *owner;
  const VtblEntry *entry;
  void *packet;

  if (g_uiMsgBoxHolder != NULL && (owner = self->owner) != NULL) {
    entry = ((GfxSpriteLayer *)owner)->vtbl + 3;
    ((void (*)(void *, float))entry->fn)((char *)owner + entry->delta, self->unk10);
    packet = GfxNewRenderPacket(self->unk10 + 0.5f);
    GfxRectDrawInPacket(self->highlight, packet);
  }
}
