// bdc 0x0889dcac BtlStageCreateBoxStopWalls
#include "bdc.h"

/* Builds the four invisible-wall sprites around the arena box (`BtlStageGetArenaBounds`) with
   `BtlStageCreateBoxWalls` (texture repeat 24.0), then sets the mipmap parameters of the
   `StopWall` texture: bias -1.0 and slope -0.004 stored in its TIM2 picture header and applied
   with `GfxTextureSetMipSlope` and `GfxTextureSetMipBias`. Fallback of
   `BtlStageCreateStopWalls` when the arena has no wall polygon. */

void BtlStageCreateBoxStopWalls(void)
{
    GfxTexture *tex;

    BtlStageCreateBoxWalls(24.0f, BtlStageGetArenaBounds());
    tex = (GfxTexture *)GfxFindTexture("StopWall");
    tex->picture->mipBias = -1.0f;
    tex->picture->mipSlope = -0.00400000019f; /* 0xbb83126f */
    GfxTextureSetMipSlope(tex->picture->mipSlope, tex);
    GfxTextureSetMipBias(tex->picture->mipBias, tex);
}
