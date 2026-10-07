// bdc 0x089cea50 GfxSetupGeState
#include "bdc.h"

/* Writes the engine's default GE state into the current display list through the libgu wrappers,
   in this order: `sceGuDrawBuffer`(3 = 8888, VRAM 0, stride 512), `sceGuDispBuffer`(480, 272,
   0x88000, 512), `sceGuDepthBuffer`(0x110000, 512), dither off, `sceGuOffset`(2048 - 240,
   2048 - 136), scissor test on, `sceGuViewport`(2048, 2048, 480, 272), `sceGuDepthRange`(0xffff, 0),
   blend off, `sceGuScissor`(0, 0, 480, 272), clip planes on, `sceGuBlendFunc`(add, src alpha,
   one minus src alpha, 0, 0), `sceGuDepthFunc`(7 = greater-or-equal), depth test on,
   `sceGuShadeModel`(smooth), `sceGuTexLevelMode`(2 = slope, -8.0f), `sceGuTexSlope`(1.6f),
   `GfxGuCtxSetD4`(0), `GfxGuCtxSetD0`(0), `sceGuTexFilter`(linear, linear),
   `sceGuFrontFace`(0 = clockwise), and finally cull face on. */

void GfxSetupGeState(void)
{
  sceGuDrawBuffer(3, 0, 0x200);
  sceGuDispBuffer(480, 272, 0x88000, 0x200);
  sceGuDepthBuffer(0x110000, 0x200);
  sceGuDisable(6);                      /* GU_DITHER */
  sceGuOffset(2048 - 240, 2048 - 136);
  sceGuEnable(2);                       /* GU_SCISSOR_TEST */
  sceGuViewport(2048, 2048, 480, 272);
  sceGuDepthRange(0xffff, 0);
  sceGuDisable(4);                      /* GU_BLEND */
  sceGuScissor(0, 0, 480, 272);
  sceGuEnable(8);                       /* GU_CLIP_PLANES */
  sceGuBlendFunc(0, 2, 3, 0, 0);        /* ADD, SRC_ALPHA, ONE_MINUS_SRC_ALPHA */
  sceGuDepthFunc(7);                    /* GU_GEQUAL */
  sceGuEnable(1);                       /* GU_DEPTH_TEST */
  sceGuShadeModel(1);                   /* GU_SMOOTH */
  sceGuTexLevelMode(2, -8.0f);          /* GU_TEXTURE_SLOPE */
  sceGuTexSlope(1.6f);
  GfxGuCtxSetD4(0);
  GfxGuCtxSetD0(0);
  sceGuTexFilter(1, 1);                 /* GU_LINEAR, GU_LINEAR */
  sceGuFrontFace(0);                    /* GU_CW */
  sceGuEnable(5);                       /* GU_CULL_FACE */
}
