// bdc 0x0889ce4c BtlStageCreateStopWallSprite
#include "bdc.h"

/* Creates one stop-wall billboard: a `StopWall` sprite on `g_billboardSpriteLayer`
   (`GfxSpriteLayerCreateBillboardByName`) with blend mode 2, billboard mode 2, flags `|=
   0x10000001`, tint and alpha copied from `g_colorWhite`, billboard parameter 0 = `repeat`, a
   rotation matrix about Y by the angle `pose[3]` (radians), position `pose[0..3]` and size
   `width` × `height`. Returns the sprite. Used by `BtlStageCreateStopWalls` and
   `BtlStageCreateBoxWalls`. */
GfxSprite *BtlStageCreateStopWallSprite(float width, float height, float repeat, float *pose)
{
    GfxSprite *sprite;
    float c;
    float s;

    sprite = GfxSpriteLayerCreateBillboardByName(g_billboardSpriteLayer, "StopWall");
    sprite->blendMode = 2;
    sprite->billboardMode = 2;
    sprite->flags = sprite->flags | 0x10000001;
    sprite->tint[0] = g_colorWhite.x;
    sprite->tint[1] = g_colorWhite.y;
    sprite->tint[2] = g_colorWhite.z;
    sprite->alpha = g_colorWhite.w;
    sprite->maybe_billboardParams80[0] = repeat;
    /* vrot of pose[3] * S703 (2/pi) in quarter turns = cos/sin of pose[3] radians */
    c = __builtin_cosf(pose[3]);
    s = __builtin_sinf(pose[3]);
    sprite->matrix[0] = c;
    sprite->matrix[1] = 0.0f;
    sprite->matrix[2] = -s;
    sprite->matrix[3] = 0.0f;
    sprite->matrix[4] = 0.0f;
    sprite->matrix[5] = 1.0f;
    sprite->matrix[6] = 0.0f;
    sprite->matrix[7] = 0.0f;
    sprite->matrix[8] = s;
    sprite->matrix[9] = 0.0f;
    sprite->matrix[10] = c;
    sprite->matrix[11] = 0.0f;
    sprite->matrix[12] = 0.0f;
    sprite->matrix[13] = 0.0f;
    sprite->matrix[14] = 0.0f;
    sprite->matrix[15] = 1.0f;
    sprite->posX = pose[0];
    sprite->posY = pose[1];
    sprite->posZ = pose[2];
    sprite->posW = pose[3];
    sprite->width = width;
    sprite->height = height;
    return sprite;
}
