// bdc 0x0889df04 BtlStageCreateStopWalls
#include "bdc.h"

/* Builds the invisible boundary walls of the current arena from its wall polygon
   `g_btlArenaStopWallPolys``[``g_btlArenaIndex``]` (`{yMin, yMax, x0, z0, …, 0, 0}`, closed
   loop; NULL → no walls, yMin = yMax = 0 → `BtlStageCreateBoxStopWalls`): one wall sprite per
   edge (`BtlStageCreateStopWallSprite`: centred on the edge midpoint at height yMin, heading
   `-atan2f(cur.z - next.z, cur.x - next.x)`, size edge length × (yMax − yMin), U scale
   `floor(length * 0.006 + 0.5)`), whose Bezier patch subdivisions it then sets (`patch.divS =
   min(length * 0.008, 64)`, `patch.divT = min(height * 0.008, 64)`). Finally it sets the `StopWall`
   texture's mip slope -0.004 and mip bias -1.0 (picture header, then `GfxTextureSetMipSlope` /
   `GfxTextureSetMipBias`). Called by `BtlStageLoadMap`. */

/* End-of-polygon test: both words of the point are integer zero (lw/lw/or). */
static int PolyPointIsEnd(const float *p)
{
  u32 bits[2];

  __builtin_memcpy(bits, p, sizeof bits);
  return (bits[0] | bits[1]) == 0;
}

void BtlStageCreateStopWalls(void)
{
  float cur[4];   /* sp+0x00: {x, yMin, z, 0} */
  float next[4];  /* sp+0x10 */
  float pose[4];  /* sp+0x20: {mid.x, mid.y, mid.z, heading} */
  const float *poly;
  const float *pts;
  const float *nextPt;
  GfxSprite *sprite;
  GfxTexture *tex;
  s32 i;
  s32 k;
  s32 more;
  float height;
  float heightDiv;
  float length;
  float lengthDiv;
  float dx;
  float dz;

  poly = g_btlArenaStopWallPolys[g_btlArenaIndex];
  if (poly == NULL) {
    return;
  }
  if (PolyPointIsEnd(poly)) {
    BtlStageCreateBoxStopWalls();
    return;
  }

  i = 0;
  more = 1;
  height = poly[1] - poly[0];
  /* sv.q of the bank constant C720 = (0, 0, 0, 0) seeds both points. */
  for (k = 0; k < 4; k++) {
    cur[k] = 0.0f;
    next[k] = 0.0f;
  }
  next[1] = poly[0];
  cur[1] = poly[0];
  pts = poly + 2;
  heightDiv = height * 0.008f;
  nextPt = pts;
  do {
    cur[0] = nextPt[0];
    i++;
    cur[2] = nextPt[1];
    nextPt = pts + i * 2;
    if (PolyPointIsEnd(nextPt)) {
      i = 0;
      more = 0;
      nextPt = pts;
    }
    next[0] = nextPt[0];
    next[2] = nextPt[1];

    /* pose = cur + (next - cur) * 0.5 (vsub.q / vscl.q / vadd.q, all four lanes). */
    for (k = 0; k < 4; k++) {
      pose[k] = cur[k] + (next[k] - cur[k]) * 0.5f;
    }

    /* length = |(cur - next).xz| (vdot.t with lane y zeroed, vsqrt.s). */
    dx = cur[0] - next[0];
    dz = cur[2] - next[2];
    length = __builtin_sqrtf(dx * dx + 0.0f * 0.0f + dz * dz);

    pose[3] = -atan2f(cur[2] - next[2], cur[0] - next[0]);

    sprite = BtlStageCreateStopWallSprite(
        length, height, (float)(s32)__builtin_floorf(length * 0.006f + 0.5f) /* floor.w.s */, pose);
    lengthDiv = length * 0.008f;
    if (lengthDiv <= 64.0f) {
      sprite->patch.divS = (s32)lengthDiv;
    } else {
      sprite->patch.divS = (s32)64.0f;
    }
    if (heightDiv <= 64.0f) {
      sprite->patch.divT = (s32)heightDiv;
    } else {
      sprite->patch.divT = (s32)64.0f;
    }
  } while (more != 0);

  tex = (GfxTexture *)GfxFindTexture("StopWall");
  tex->picture->mipBias = -1.0f;
  tex->picture->mipSlope = -0.00400000019f; /* 0xbb83126f */
  GfxTextureSetMipSlope(tex->picture->mipSlope, tex);
  GfxTextureSetMipBias(tex->picture->mipBias, tex);
}
