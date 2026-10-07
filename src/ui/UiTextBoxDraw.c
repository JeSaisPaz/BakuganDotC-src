// bdc 0x089eb264 UiTextBoxDraw
#include "bdc.h"

/* Draws the text box: when it has a printer, opens a render packet at the box depth (`+0x0`,
   `GfxNewRenderPacket`) and draws the printer into it (`GfxSpriteLayerDraw`), then marks the box as
   drawn (`+0x8 = 1`) so the next `UiTextBoxPrint` starts from an empty buffer. */

void UiTextBoxDraw(UiTextBox *box)

{
  void *packet;
  UiTextPrinter *self;
  
  self = box->printer;
  if (self != (UiTextPrinter *)0x0) {
    packet = GfxNewRenderPacket(box->packetDepth);
    GfxSpriteLayerDraw(&self->layer,packet);
    box->drawn = '\x01';
  }
  return;
}

