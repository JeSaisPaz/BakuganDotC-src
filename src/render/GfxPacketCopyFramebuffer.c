// bdc 0x089f1780 GfxPacketCopyFramebuffer
#include "bdc.h"

/* Emits into a render packet the GE commands that copy a downscaled image of the frame being drawn
   (the back buffer, `sceGeEdramGetAddr() + (g_gfxFrameIndex ^ 1) * 0x88000`, 512-wide 8888) into
   the 256x256 565 buffer `dst` (defaults to the free VRAM after the frame buffers,
   `g_gfxDisplay->vramFreeStart`): enables texturing, disables alpha test / depth / stencil /
   blending, sets the draw buffer to `dst` with a 256x256 viewport and scissor, binds the back
   buffer as a 512x512 texture and draws the four sprite strips of `g_gfxFbCopyVerts` (480x272 ->
   256x256), then restores the 480x272 viewport / scissor and the 8888 draw buffer. Used by screen
   effects that sample the frame (blur, fades, transitions). */

/* GE float argument: the top 24 bits of the IEEE single. */
static inline u32 GfxCopyFloatArg(float f)
{
  union { float f; u32 u; } bits;
  bits.f = f;
  return bits.u >> 8;
}

/* log2 of a power of two, as the 31 - clz the asm computes. */
static inline u32 GfxCopyLog2(u32 n)
{
  return 31 - __builtin_clz(n);
}

void GfxPacketCopyFramebuffer(void *packet, void *dst)
{
  RenderPacket *pkt = (RenderPacket *)packet;
  u32 *dl;
  u32 *start;
  uintptr_t dstAddr;
  uintptr_t texAddr;
  uintptr_t verts;
  u32 tsize;

  if (dst == NULL) {
    dst = g_gfxDisplay->vramFreeStart;
  }
  dstAddr = (uintptr_t)dst;
  start = GfxPacketBeginChunk(pkt);
  start[0] = 0x1e000001; /* TME on */
  start[1] = 0x22000000; /* ATE off */
  start[2] = 0x23000000; /* ZTE off */
  start[3] = 0x24000000; /* STE off */
  start[4] = 0x21000000; /* ABE off */
  dl = start + 5;
  dl[0] = 0x55ffffff; /* AMC white */
  dl[1] = 0x580000ff; /* AMA 0xff */
  dl += 2;
  dl[0] = 0x4c000000; /* OFFSETX 0 */
  dl[1] = 0x4d000000; /* OFFSETY 0 */
  dl += 2;
  dl[0] = GfxCopyFloatArg(1.0f) | 0x48000000; /* USCALE */
  dl[1] = GfxCopyFloatArg(1.0f) | 0x49000000; /* VSCALE */
  dl += 2;
  dl[0] = GfxCopyFloatArg(0.0f) | 0x4a000000; /* UOFFSET */
  dl[1] = GfxCopyFloatArg(0.0f) | 0x4b000000; /* VOFFSET */
  dl += 2;
  dl[0] = 0xd2000000;                                          /* PSM 565 */
  dl[1] = (((u32)(dstAddr >> 24) & 0xf) << 16) | 0x9d000100; /* FBW 256 */
  dl[2] = ((u32)dstAddr & 0xffffff) | 0x9c000000;              /* FBP dst */
  dl += 3;
  dl[0] = GfxCopyFloatArg(128.0f) | 0x42000000;  /* XSCALE */
  dl[1] = GfxCopyFloatArg(-128.0f) | 0x43000000; /* YSCALE */
  dl[2] = GfxCopyFloatArg(0.0f) | 0x45000000;    /* XPOS */
  dl[3] = GfxCopyFloatArg(0.0f) | 0x46000000;    /* YPOS */
  dl += 4;
  dl[0] = 0xd4000000; /* SCISSOR1 (0,0) */
  dl[1] = 0xd503fcff; /* SCISSOR2 (255,255) */
  dl[2] = 0x15000000; /* REGION1 (0,0) */
  dl[3] = 0x1603fcff; /* REGION2 (255,255) */
  dl += 4;
  texAddr = (uintptr_t)sceGeEdramGetAddr() + (g_gfxFrameIndex ^ 1) * 0x88000;
  tsize = (GfxCopyLog2(0x200) << 8) | 0xb8000000 | GfxCopyLog2(0x200);
  dl[0] = (((u32)(texAddr >> 24) & 0xf) << 16) | 0xa8000200; /* TBW0 512 */
  dl[1] = ((u32)texAddr & 0xffffff) | 0xa0000000;              /* TBP0 */
  dl[2] = tsize;                                               /* TSIZE0 512x512 */
  dl[3] = 0xcb000000;                                          /* TFLUSH */
  dl += 4;
  dl[0] = 0xc2000000; /* TMODE */
  dl[1] = 0xc3000003; /* TPSM 8888 */
  dl[2] = 0xcb000000; /* TFLUSH */
  dl += 3;
  dl[0] = 0x12800102; /* VTYPE: through, 16-bit UV, 16-bit position */
  dl += 1;
  verts = (uintptr_t)g_gfxFbCopyVerts;
  dl[0] = (((u32)(verts >> 24) & 0xf) << 16) | 0x10000000; /* BASE */
  dl[1] = ((u32)verts & 0xffffff) | 0x01000000;              /* VADDR */
  dl += 2;
  dl[0] = 0x04060008; /* PRIM sprites, 8 vertices */
  dl += 1;
  dl[0] = 0x4c007100; /* OFFSETX 2048 - 240 */
  dl[1] = 0x4d007780; /* OFFSETY 2048 - 136 */
  dl += 2;
  dl[0] = GfxCopyFloatArg(240.0f) | 0x42000000;  /* XSCALE */
  dl[1] = GfxCopyFloatArg(-136.0f) | 0x43000000; /* YSCALE */
  dl[2] = GfxCopyFloatArg(2048.0f) | 0x45000000; /* XPOS */
  dl[3] = GfxCopyFloatArg(2048.0f) | 0x46000000; /* YPOS */
  dl += 4;
  dl[0] = 0xd4000000; /* SCISSOR1 (0,0) */
  dl[1] = 0xd5043ddf; /* SCISSOR2 (479,271) */
  dl[2] = 0x15000000; /* REGION1 (0,0) */
  dl[3] = 0x16043ddf; /* REGION2 (479,271) */
  dl += 4;
  if (g_gfxFrameIndex != 0) {
    dl[0] = 0xd2000003; /* PSM 8888 */
    dl[1] = 0x9d000200; /* FBW 512 */
    dl[2] = 0x9c000000; /* FBP 0 */
  } else {
    dl[0] = 0xd2000003; /* PSM 8888 */
    dl[1] = 0x9d000200; /* FBW 512 */
    dl[2] = 0x9c088000; /* FBP 0x88000 */
  }
  dl += 3;
  GfxPacketEndChunk(pkt, dl);
}
