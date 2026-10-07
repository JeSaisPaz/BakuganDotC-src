// bdc 0x08a1f3b0 GfxDlWriteEnableDisable
#include "bdc.h"

/* libgu internal state switch behind `sceGuEnable`/`sceGuDisable`: for GU state `state` (0..21,
   the `GU_*` enable constants of `pspgu.h`) writes the matching GE enable command with argument
   `enable` into `ctx`'s list (`ctx->listCurrent`): 0 ALPHA_TEST 0x22, 1 DEPTH_TEST 0x23, 3 STENCIL_TEST 0x24,
   4 BLEND 0x21, 5 CULL_FACE 0x1d, 6 DITHER 0x20, 7 FOG 0x1f, 8 CLIP_PLANES 0x1c, 9 TEXTURE_2D 0x1e,
   10 LIGHTING 0x17, 11..14 LIGHT0..3 0x18..0x1b, 15 LINE_SMOOTH 0x25, 16 PATCH_CULL 0x26, 17
   COLOR_TEST 0x27, 18 COLOR_LOGIC_OP 0x28, 19 FACE_NORMAL_REVERSE 0x51, 20 PATCH_FACE 0x38. State 2
   (SCISSOR_TEST) rewrites the scissor (`0xd4`/`0xd5`) and region (`0x15`/`0x16`) from the saved
   scissor rectangle (`scissorX0..scissorY1`) or the full draw area (`dispWidth`/`dispHeight`) and stores the flag
   in `scissorEnable`; state 21 (FRAGMENT_2X) updates `fragment2x` and rewrites TEXFUNC (`0xc9`). */

void GfxDlWriteEnableDisable(GuContext *ctx, s32 state, u32 enable)
{
  u32 *p;
  u32 cmd;
  u32 rect;

  switch (state) {
  case 0:  cmd = 0x22000000; break; /* ALPHA_TEST */
  case 1:  cmd = 0x23000000; break; /* DEPTH_TEST */
  case 2:                            /* SCISSOR_TEST */
    if (enable != 0) {
      p = (u32 *)ctx->listCurrent;
      rect = ctx->scissorY1 << 10 | ctx->scissorX1;
      ctx->scissorEnable = 1;
      p[0] = ctx->scissorY0 << 10 | ctx->scissorX0 | 0xd4000000;
    } else {
      s32 height = ctx->dispHeight;
      p = (u32 *)ctx->listCurrent;
      s32 width = ctx->dispWidth;
      ctx->scissorEnable = 0;
      p[0] = 0xd4000000;
      rect = (height - 1) << 10 | (width - 1);
    }
    ctx->listCurrent = (u8 *)(p + 4);
    p[1] = rect | 0xd5000000;
    p[2] = 0x15000000;
    p[3] = rect | 0x16000000;
    return;
  case 3:  cmd = 0x24000000; break; /* STENCIL_TEST */
  case 4:  cmd = 0x21000000; break; /* BLEND */
  case 5:  cmd = 0x1d000000; break; /* CULL_FACE */
  case 6:  cmd = 0x20000000; break; /* DITHER */
  case 7:  cmd = 0x1f000000; break; /* FOG */
  case 8:  cmd = 0x1c000000; break; /* CLIP_PLANES */
  case 9:  cmd = 0x1e000000; break; /* TEXTURE_2D */
  case 10: cmd = 0x17000000; break; /* LIGHTING */
  case 11: cmd = 0x18000000; break; /* LIGHT0 */
  case 12: cmd = 0x19000000; break; /* LIGHT1 */
  case 13: cmd = 0x1a000000; break; /* LIGHT2 */
  case 14: cmd = 0x1b000000; break; /* LIGHT3 */
  case 15: cmd = 0x25000000; break; /* LINE_SMOOTH */
  case 16: cmd = 0x26000000; break; /* PATCH_CULL_FACE */
  case 17: cmd = 0x27000000; break; /* COLOR_TEST */
  case 18: cmd = 0x28000000; break; /* COLOR_LOGIC_OP */
  case 19: cmd = 0x51000000; break; /* FACE_NORMAL_REVERSE */
  case 20: cmd = 0x38000000; break; /* PATCH_FACE */
  case 21:                           /* FRAGMENT_2X */
    ctx->fragment2x = (enable != 0) ? 1 : 0;
    p = (u32 *)ctx->listCurrent;
    p[0] = ctx->fragment2x << 16 | ctx->texComponent << 8 | ctx->texFunction | 0xc9000000;
    ctx->listCurrent = (u8 *)(p + 1);
    return;
  default:
    return;
  }
  p = (u32 *)ctx->listCurrent;
  p[0] = enable | cmd;
  ctx->listCurrent = (u8 *)(p + 1);
}
