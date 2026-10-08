// bdc 0x089f26b4 GfxBeginFrame
#include "bdc.h"

/* Starts the next frame's display list: toggles the double-buffer index (`g_gfxFrameIndex`), selects
   that half of the 2x0x60000-byte list buffer (`g_renderListBuffer`), `sceGuStart`s context 4 on
   it, writes PSM/FBW/FBP for the draw buffer and then clears the frame by drawing 16 sprites
   (32 x 272 px, GE clear mode) in the `GfxDisplay` clear colour (`clearColor`, each channel clamped
   to [0,1], scaled by 255 and packed as RGBA8888). Leaves the list pointer after the clear in
   `g_renderListCursor`. */

void GfxBeginFrame(GfxDisplay *display)
{
    u32 fbp;
    u32 colour;
    u32 *list;
    u32 *end;
    GfxGeColorVertex16 *verts;
    GfxGeColorVertex16 *v;
    int i;

    g_gfxFrameIndex ^= 1;
    g_renderListCursor = (u32 *)((u8 *)g_renderListBuffer + g_gfxFrameIndex * 0x60000);
    g_renderListStart = g_renderListCursor;
    sceGuStart(4, g_renderListCursor, 0x60000);
    g_renderListCursor = GfxGuGetListPtr(NULL);

    fbp = 0;
    if (g_gfxFrameIndex == 0) {
        fbp = 0x88000;
    }
    g_renderListCursor[0] = 0xd2000003;                                   /* PSM 8888 */
    g_renderListCursor[1] = ((fbp >> 24 & 0xf) << 16) | 0x9d000200;      /* FBW 512 */
    g_renderListCursor[2] = (fbp & 0xffffff) | 0x9c000000;               /* FBP */
    list = g_renderListCursor + 3;
    g_renderListCursor = list;

    /* vsat0.q, vscl.q by S701 (255), vf2iz.q 23, vi2uc.q */
    colour = (u32)VfI2uc(VfF2iz(VfSat0(display->clearColor[0]) * 255.0f, 23)) |
             (u32)VfI2uc(VfF2iz(VfSat0(display->clearColor[1]) * 255.0f, 23)) << 8 |
             (u32)VfI2uc(VfF2iz(VfSat0(display->clearColor[2]) * 255.0f, 23)) << 16 |
             (u32)VfI2uc(VfF2iz(VfSat0(display->clearColor[3]) * 255.0f, 23)) << 24;

    /* Jump over the inline vertex data. */
    verts = (GfxGeColorVertex16 *)(list + 2);
    end = (u32 *)((u8 *)verts + 32 * sizeof(GfxGeColorVertex16));
    list[0] = ((PspAddr(end) >> 24 & 0xf) << 16) | 0x10000000;          /* BASE */
    list[1] = (PspAddr(end) & 0xffffff) | 0x08000000;                    /* JUMP */

    v = verts;
    for (i = 0; i < 32; i++) {
        v->colour = colour;
        v->x = (s16)((i / 2 + i % 2) * 32);
        v->y = (s16)((i % 2) * 272);
        v->z = 0;
        v++;
    }

    end[0] = 0xd3000701;                                                  /* CLEAR on */
    end[1] = 0x1280011c;                                                  /* VTYPE through, 8888, s16 */
    end += 2;
    if (verts != NULL) {
        end[0] = ((PspAddr(verts) >> 24 & 0xf) << 16) | 0x10000000;        /* BASE */
        end[1] = (PspAddr(verts) & 0xffffff) | 0x01000000;                 /* VADDR */
        end += 2;
    }
    end[0] = 0x04060020;                                                  /* PRIM sprites x32 */
    end[1] = 0xd3000000;                                                  /* CLEAR off */
    g_renderListCursor = end + 2;
}
