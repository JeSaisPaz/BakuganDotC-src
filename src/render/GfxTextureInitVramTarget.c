// bdc 0x089f727c GfxTextureInitVramTarget
#include "bdc.h"

/* Initialises a texture object that samples a VRAM buffer (render-to-texture, e.g.
   `"FeedBackTex"`): uses `vram` or, when NULL, the display's free-VRAM start; zeroes the
   sub-records, builds a fake 256x256 TIM2 image in the static `g_gfxVramTargetTim2`, copies the
   name, sets `1/256` UV factors and an inline GE block: TMODE, TPSM, TFLUSH, TBP0/TBW0 = `vram`
   width 256, TSIZE0 256x256 (`0xb8000808`), TFLUSH, TFILTER linear (`0xc6000101`), TWRAP clamp
   (`0xc7000101`), RET. Returns nothing. */

void GfxTextureInitVramTarget(void *tex, char *name, void *vram)
{
    GfxTexture *t = (GfxTexture *)tex;
    u32 width;
    u32 height;
    u32 *cmd;

    if (vram == NULL) {
        vram = g_gfxDisplay->vramFreeStart;
    }
    t->singleSlot = 1;
    t->blocks = t->inlineBlock;
    memset(&t->rec38Type, 0, 0x20);
    t->rec38Type = 1;
    t->rec38Owner = t;
    memset(t->gmoData, 0, 0x40);
    t->gmoData[0] = 1;
    t->gmoOwner = t;
    t->clutData = NULL;
    t->tim2 = g_gfxVramTargetTim2;
    memset(g_gfxVramTargetTim2, 0, sizeof(g_gfxVramTargetTim2));
    t->ownsTim2 = 0;
    t->picture = (GfxTim2Picture *)((GfxTim2Header *)t->tim2 + 1);
    t->picture->imageType = 0;
    strcpy(t->name, name);
    t->picture->width = 0x100;
    t->picture->height = 0x100;
    t->unk11c = vram;
    t->vramBlock1 = NULL;
    t->vramBlock0 = NULL;
    width = GfxTextureRoundPow2(t, GfxTextureGetWidth(t));
    height = GfxTextureRoundPow2(t, GfxTextureGetHeight(t));
    t->isPow2Size = 1;
    cmd = (u32 *)t->blocks;
    t->curBlock = t->blocks;
    t->curBlock2 = t->blocks;
    t->invWidth = 1.0f / (float)(s32)width;
    t->invHeight = 1.0f / (float)(s32)height;
    cmd[0] = 0xc2000000;                                           /* TMODE */
    cmd[1] = 0xc3000000;                                           /* TPSM */
    cmd[2] = 0xcb000000;                                           /* TFLUSH */
    cmd[3] = 0xa8000100 | (((PspAddr(vram) >> 24) & 0xf) << 16);  /* TBW0 256, TBP0 high */
    cmd[4] = 0xa0000000 | (PspAddr(vram) & 0xffffff);             /* TBP0 low */
    cmd[5] = 0xb8000808;                                           /* TSIZE0 256x256 */
    cmd[6] = 0xcb000000;                                           /* TFLUSH */
    cmd[7] = 0xc6000101;                                           /* TFILTER linear */
    cmd[8] = 0xc7000101;                                           /* TWRAP clamp */
    cmd[9] = 0x0b000000;                                           /* RET */
}
