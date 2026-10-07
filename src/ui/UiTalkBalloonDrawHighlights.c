// bdc 0x088cabb8 UiTalkBalloonDrawHighlights
#include "bdc.h"

/* When highlights are enabled (`pageFlags[2]`) and there is at least one, opens a chunk on
   `packet` with the 2D render state, the screen camera (`g_gfxScreenCamera`) offset by the
   printer's origin (the offset is reset to `g_gfxVecZero` afterwards), texture function
   modulate/RGBA, linear filtering and the highlight texture; then draws one bar per entry of
   `highlights` (count `highlightCount`) at x − 16, y − 14, z 0 with extra width w in
   `highlightColor` (`UiTalkBalloonDrawQuad`). Each record is copied to a stack
   vector first. */

void UiTalkBalloonDrawHighlights(UiTalkBalloon *self, void *packet)
{
  RenderPacket *pkt = packet;
  u32 *dl;
  s32 i;
  float pos[4];

  if (self->pageFlags[2] == 0 || self->highlightCount <= 0) {
    return;
  }
  dl = GfxPacketBeginChunk(pkt);
  dl = GfxDlCall2DState(dl);
  GfxScreenCameraSetViewOffset(&self->printer->layer.view.w.x);
  dl = GfxCameraDlWrite(g_gfxScreenCamera, dl, 0xffffffff);
  GfxScreenCameraSetViewOffset(&g_gfxVecZero.x);
  dl[0] = 0xc9000100; /* TFUNC: modulate, RGBA */
  dl[1] = 0xc6000101; /* TFLT: linear min/mag */
  dl = GfxTextureWriteCall(self->highlightTexture, dl + 2, 0);
  GfxPacketEndChunk(pkt, dl);
  for (i = 0; i < self->highlightCount; i++) {
    pos[0] = self->highlights[i][0];
    pos[1] = self->highlights[i][1];
    pos[2] = self->highlights[i][2];
    pos[3] = self->highlights[i][3];
    pos[0] = pos[0] - 16.0f;
    pos[1] = pos[1] - 14.0f;
    pos[2] = 0.0f;
    UiTalkBalloonDrawQuad(pos[3], pkt, pos, self->highlightColor);
  }
}
