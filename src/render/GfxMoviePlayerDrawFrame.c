// bdc 0x089d5ad8 GfxMoviePlayerDrawFrame
#include "bdc.h"

/* Emits the GE commands that blit the decoded video frame: opens a render packet just above the
   active fader, sets texture mode/format 8888 (`TPSM 3`), the frame buffer `videoData->displaybuf`
   as texture 0 (512x512, `TBW 0x200`), and draws the 30 through-mode sprite strips of the player's
   strip table (`vertices`, see `GfxMoviePlayerInit`) with `PRIM 6, 30`. With no frame buffer it
   disables texturing instead. */

void GfxMoviePlayerDrawFrame(GfxMoviePlayer *player)

{
  GfxFader *fader;
  void *packet;
  u32 *list;
  u32 *after;
  uintptr_t tex;
  uintptr_t verts;

  fader = GfxGetActiveFader();
  packet = GfxNewRenderPacket(fader->sortKey + 1.0f);
  list = GfxPacketBeginChunk(packet);
  list = GfxDlCall2DState(list);
  list[0] = 0x21000000;
  list[1] = 0x22000000;
  if (player->videoData->displaybuf == NULL) {
    list[2] = 0x1e000000;                       /* TME off */
    after = list + 3;
    list = list + 5;
  }
  else {
    list[2] = 0xc2000000;                       /* TMODE */
    list[3] = 0xc3000003;                       /* TPSM 8888 */
    list[4] = 0xcb000000;                       /* TFLUSH */
    tex = (uintptr_t)player->videoData->displaybuf;
    list[5] = (((u32)(tex >> 24) & 0xf) << 16) | 0xa8000200;   /* TBW0 512 */
    list[6] = ((u32)tex & 0xffffff) | 0xa0000000;               /* TBP0 */
    list[7] = 0xb8000000 | (9 << 8) | 9;        /* TSIZE0 512x512 */
    list[8] = 0xcb000000;                       /* TFLUSH */
    after = list + 9;
    list = list + 11;
  }
  after[0] = 0x55ffffff;
  after[1] = 0x580000ff;
  verts = (uintptr_t)player->vertices;
  list[0] = 0x12800102;                         /* VTYPE */
  after = list + 1;
  if (verts != 0) {
    after[0] = (((u32)(verts >> 24) & 0xf) << 16) | 0x10000000; /* BASE */
    list[2] = ((u32)verts & 0xffffff) | 0x01000000;             /* VADDR */
    after = list + 3;
  }
  after[0] = 0x0406001e;                        /* PRIM sprites, 30 */
  after[1] = 0x21000001;
  GfxPacketEndChunk(packet, after + 2);
  return;
}
