// bdc 0x0889cf04 BtlStageCreateBoxWalls
#include "bdc.h"

/* Creates the four wall billboards of an axis-aligned arena box `box = {x0, y0, z0, x1, y1, z1}`
   with `BtlStageCreateStopWallSprite`, all `y1 − y0` high and placed at height `y0`: two walls
   `x1 − x0` wide centred on x at `z0` (angle 0) and `z1` (angle π), then two walls `z1 − z0` wide
   centred on z at `x0` (angle π/2) and `x1` (angle −π/2). Called by `BtlStageCreateBoxStopWalls`
   with `repeat = 24.0`. */
void BtlStageCreateBoxWalls(float repeat, float *box)
{
    float pose[4] __attribute__((aligned(16)));
    float width;
    float height;

    height = box[4] - box[1];
    width = box[3] - box[0];
    pose[0] = (box[0] + box[3]) * 0.5f;
    pose[1] = box[1];
    pose[2] = box[2];
    pose[3] = 0.0f;
    BtlStageCreateStopWallSprite(width, height, repeat, pose);
    pose[2] = box[5];
    pose[3] = 3.14159274f;
    BtlStageCreateStopWallSprite(width, height, repeat, pose);
    width = box[5] - box[2];
    pose[0] = box[0];
    pose[2] = (box[2] + box[5]) * 0.5f;
    pose[3] = 1.57079637f;
    BtlStageCreateStopWallSprite(width, height, repeat, pose);
    pose[0] = box[3];
    pose[3] = -1.57079637f;
    BtlStageCreateStopWallSprite(width, height, repeat, pose);
}
