// bdc 0x0882b354 GfxPlayerBlurTaskDraw
#include "bdc.h"

/* Draw of the player blur task (`GfxPlayerBlurTaskCtor`) (vtable slot 4); does nothing once `state`
   reaches 3. Otherwise it queues a render packet (sort key -100) that: switches the GE to a
   256x256 target (offset/viewport/scissor/region, PSM 565, FBW 256 on the current draw buffer
   picked by `g_gfxFrameIndex`) and calls the 2D state list (`GfxDlCall2DState`); clears it to
   black (`GfxPacketDrawRect` with `g_playerBlurClearRect`); applies the feedback pass
   (`GfxPlayerBlurWriteFeedbackPass`) when `feedbackEnabled` and `frameCounter >= 2`; writes the
   lighting state for `camera` with `lightDir` temporarily swapped into `g_gfxLightDir0`; draws
   `model` through its vtable; clears depth with 16 GE clear-mode sprites; appends SIGNAL/END and
   FINISH/END once `frameCounter >= 2`; clears the rectangle again and restores the 480x272
   8888/FBW 512 target. If `unk547` is set, a second packet (sort key 0.3) gets the
   composite pass (`GfxPlayerBlurWriteCompositePass`). Finally `g_blurCaptureActive` is set to 1
   when `frameCounter` differs from `g_playerBlurLastFrame` (which takes the new value), else 0. */

/* GE float argument: the top 24 bits of the IEEE single. */
static inline u32 GfxBlurFloatArg(float f)
{
    union { float f; u32 u; } bits;
    bits.f = f;
    return bits.u >> 8;
}

void GfxPlayerBlurTaskDraw(void *task)
{
    GfxPlayerBlurTask *self = (GfxPlayerBlurTask *)task;
    float savedLight[4];
    RenderPacket *packet;
    RenderPacket *compPacket;
    const GfxModelVtable *vt;
    GfxCamera *savedCamera;
    GfxGeColorVertex16 *verts;
    GfxGeColorVertex16 *v;
    u32 *dl;
    u32 *end;
    u32 *comp;
    int i;

    if (self->state >= 3) {
        return;
    }

    /* Render target: 256x256, PSM 565, FBW 256. */
    packet = (RenderPacket *)GfxNewRenderPacket(-100.0f);
    dl = GfxPacketBeginChunk(packet);
    dl[0] = 0x4c007800; /* OFFSETX 2048 - 128 */
    dl[1] = 0x4d007800; /* OFFSETY 2048 - 128 */
    dl += 2;
    dl[0] = GfxBlurFloatArg(128.0f) | 0x42000000;  /* XSCALE */
    dl[1] = GfxBlurFloatArg(-128.0f) | 0x43000000; /* YSCALE */
    dl[2] = GfxBlurFloatArg(2048.0f) | 0x45000000; /* XPOS */
    dl[3] = GfxBlurFloatArg(2048.0f) | 0x46000000; /* YPOS */
    dl += 4;
    dl[0] = 0xd4000000; /* SCISSOR1 (0,0) */
    dl[1] = 0xd503fcff; /* SCISSOR2 (255,255) */
    dl[2] = 0x15000000; /* REGION1 (0,0) */
    dl[3] = 0x1603fcff; /* REGION2 (255,255) */
    dl += 4;
    if (g_gfxFrameIndex != 0) {
        dl[0] = 0xd2000000; /* PSM 565 */
        dl[1] = 0x9d000100; /* FBW 256 */
        dl[2] = 0x9c000000; /* FBP 0 */
        dl += 3;
    } else {
        dl[0] = 0xd2000000; /* PSM 565 */
        dl[1] = 0x9d000100; /* FBW 256 */
        dl[2] = 0x9c088000; /* FBP 0x88000 */
        dl += 3;
    }
    dl = GfxDlCall2DState(dl);
    GfxPacketEndChunk(packet, dl);

    if (g_playerBlurClearRectInit == 0) {
        g_playerBlurClearRectInit = 1;
        g_playerBlurClearRect[0] = 0.0f;
        g_playerBlurClearRect[1] = 0.0f;
        g_playerBlurClearRect[2] = 256.0f;
        g_playerBlurClearRect[3] = 256.0f;
    }
    GfxPacketDrawRect(packet, g_playerBlurClearRect, (const float *)&g_colorBlack);

    dl = GfxPacketBeginChunk(packet);
    if (self->feedbackEnabled != 0 && self->frameCounter >= 2) {
        dl = GfxPlayerBlurWriteFeedbackPass(task, dl, self->texture);
    }

    /* Light the model with the task's own direction (quad copies in the original). */
    savedCamera = g_gfxActiveCamera;
    for (i = 0; i < 4; i++) {
        savedLight[i] = g_gfxLightDir0[i];
    }
    for (i = 0; i < 4; i++) {
        g_gfxLightDir0[i] = self->lightDir[i];
    }
    dl = GfxDlWriteLightState(dl, self->camera, 1);
    for (i = 0; i < 4; i++) {
        g_gfxLightDir0[i] = savedLight[i];
    }
    g_gfxActiveCamera = savedCamera;

    vt = (const GfxModelVtable *)self->model->base.vtable;
    vt->draw((u8 *)self->model + vt->drawAdjust, &dl);

    /* Depth clear: 16 sprites (32 x 272 px) of inline vertices; the colour word is left as is. */
    verts = (GfxGeColorVertex16 *)(dl + 2);
    end = (u32 *)((u8 *)verts + 32 * sizeof(GfxGeColorVertex16));
    dl[0] = (((u32)(uintptr_t)end >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
    dl[1] = ((u32)(uintptr_t)end & 0xffffff) | 0x08000000;          /* JUMP */
    v = verts;
    for (i = 0; i < 32; i++) {
        v->x = (s16)((i / 2 + i % 2) * 32);
        v->y = (s16)((i % 2) * 272);
        v->z = 0;
        v++;
    }
    end[0] = 0xd3000401; /* CLEAR on, depth only */
    end[1] = 0x1280011c; /* VTYPE through, 8888, s16 */
    end += 2;
    if (verts != NULL) {
        end[0] = (((u32)(uintptr_t)verts >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
        end[1] = ((u32)(uintptr_t)verts & 0xffffff) | 0x01000000;          /* VADDR */
        end += 2;
    }
    end[0] = 0x04060020; /* PRIM sprites x32 */
    end[1] = 0xd3000000; /* CLEAR off */
    dl = end + 2;

    if (self->frameCounter >= 2) {
        dl[0] = 0x0e030000; /* SIGNAL 3 */
        dl[1] = 0x0c000000; /* END */
        dl[2] = 0x0f000000; /* FINISH */
        dl[3] = 0x0c000000; /* END */
        dl += 4;
    }
    GfxPacketEndChunk(packet, dl);
    GfxPacketDrawRect(packet, g_playerBlurClearRect, (const float *)&g_colorBlack);

    /* Restore the screen target: 480x272, PSM 8888, FBW 512. */
    dl = GfxPacketBeginChunk(packet);
    dl[0] = 0x4c007100; /* OFFSETX 2048 - 240 */
    dl[1] = 0x4d007780; /* OFFSETY 2048 - 136 */
    dl += 2;
    dl[0] = GfxBlurFloatArg(240.0f) | 0x42000000;  /* XSCALE */
    dl[1] = GfxBlurFloatArg(-136.0f) | 0x43000000; /* YSCALE */
    dl[2] = GfxBlurFloatArg(2048.0f) | 0x45000000; /* XPOS */
    dl[3] = GfxBlurFloatArg(2048.0f) | 0x46000000; /* YPOS */
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
        dl += 3;
    } else {
        dl[0] = 0xd2000003; /* PSM 8888 */
        dl[1] = 0x9d000200; /* FBW 512 */
        dl[2] = 0x9c088000; /* FBP 0x88000 */
        dl += 3;
    }
    GfxPacketEndChunk(packet, dl);

    if (self->unk547 != 0) {
        compPacket = (RenderPacket *)GfxNewRenderPacket(0.3f);
        comp = GfxPacketBeginChunk(compPacket);
        comp = GfxPlayerBlurWriteCompositePass(task, comp, self->texture);
        GfxPacketEndChunk(compPacket, comp);
    }

    if (g_playerBlurLastFrame == self->frameCounter) {
        g_blurCaptureActive = 0;
    } else {
        g_playerBlurLastFrame = self->frameCounter;
        g_blurCaptureActive = 1;
    }
}
