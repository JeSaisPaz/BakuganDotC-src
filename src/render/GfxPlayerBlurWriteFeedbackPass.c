// bdc 0x0882b074 GfxPlayerBlurWriteFeedbackPass
#include "bdc.h"

/* Feedback pass of the player blur task (`GfxPlayerBlurTaskCtor`): after
   `GfxPlayerBlurWriteTexState` sets normal alpha blending (src alpha / 1 - src alpha) with
   material colour white, alpha 0.94, and draws the blur texture over itself as 32 vertical
   8-pixel sprite strips (256×256, vertex table `feedbackVerts` built once), so the old image
   decays. Returns the command pointer. */

u32 * GfxPlayerBlurWriteFeedbackPass(void *task, u32 *dl, void *tex)

{
    GfxPlayerBlurTask *self = (GfxPlayerBlurTask *)task;
    float color[4] __attribute__((aligned(16)));
    GfxStripVertex *verts;
    u32 packed;
    int i;

    dl = GfxPlayerBlurWriteTexState(task, dl, tex);
    dl[0] = 0xdf000032; /* BLEND: src alpha, 1 - src alpha, add */
    dl[1] = 0xe0000000; /* FIXA */
    dl[2] = 0xe1000000; /* FIXB */
    dl += 3;

    color[0] = 1.0f;
    color[1] = 1.0f;
    color[2] = 1.0f;
    color[3] = 0.94f;
    /* vsat0.q, vscl.q by S701 (255), vf2iz.q 23, vi2uc.q */
    packed = (u32)VfI2uc(VfF2iz(VfSat0(color[0]) * 255.0f, 23)) |
             (u32)VfI2uc(VfF2iz(VfSat0(color[1]) * 255.0f, 23)) << 8 |
             (u32)VfI2uc(VfF2iz(VfSat0(color[2]) * 255.0f, 23)) << 16 |
             (u32)VfI2uc(VfF2iz(VfSat0(color[3]) * 255.0f, 23)) << 24;
    dl[0] = (packed & 0xffffff) | 0x55000000; /* material colour */
    dl[1] = (packed >> 24) | 0x58000000;      /* material alpha */
    dl += 2;

    verts = (GfxStripVertex *)self->feedbackVerts;
    if (self->feedbackBuilt == 0) {
        self->feedbackBuilt = 1;
        for (i = 0; i < 64; i += 2) {
            verts[i].u = (u16)(i * 4);
            verts[i + 1].u = (u16)(i * 4 + 8);
            verts[i].x = (s16)(i * 4);
            verts[i + 1].x = (s16)(i * 4 + 8);
            verts[i].v = 0;
            verts[i + 1].v = 0x100;
            verts[i].y = 0;
            verts[i + 1].y = 0x100;
            verts[i + 1].z = 0;
            verts[i].z = 0;
        }
    }

    *dl++ = 0x12800102; /* VTYPE: through, 16-bit UV and position */
    if (verts != NULL) {
        dl[0] = ((PspAddr(verts) >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
        dl[1] = (PspAddr(verts) & 0xffffff) | 0x01000000;          /* VADDR */
        dl += 2;
    }
    *dl = 0x04060040; /* PRIM: sprites, 64 vertices */
    return dl + 1;
}
