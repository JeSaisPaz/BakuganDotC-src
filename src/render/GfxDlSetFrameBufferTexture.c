// bdc 0x089f1ee8 GfxDlSetFrameBufferTexture
#include "bdc.h"

/* Writes GE commands that bind the frame buffer shown last frame (VRAM `sceGeEdramGetAddr() +
   (g_gfxFrameIndex ^ 1) * 0x88000`, 512x512 8888) as texture 0 with texturing enabled; used for
   screen-capture effects (`UiTalkBalloonDrawFrame`). Returns the list pointer past the 12 words. */

u32 *GfxDlSetFrameBufferTexture(u32 *list)
{
  u8 *vram = (u8 *)sceGeEdramGetAddr();
  u32 half = (u32)g_gfxFrameIndex ^ 1;
  u32 tex;

  list[0] = 0xcc000000;
  list[1] = 0xc6000000;
  list[2] = 0xc9000003;
  list[3] = 0xc7000101;
  list[4] = 0xc2000000;
  list[5] = 0xc3000003;
  tex = PspAddr(vram + half * 0x88000);
  list[6] = 0xcb000000;
  list[7] = ((tex >> 24 & 0xf) << 16) | 0xa8000200;
  list[8] = (tex & 0xffffff) | 0xa0000000;
  list[9] = 0xb8000909;
  list[10] = 0xcb000000;
  list[11] = 0x1e000001;
  return list + 12;
}
