// bdc 0x08a1f5bc GfxDlWriteDepthRange
#include "bdc.h"

/* libgu internal depth-range writer behind `sceGuDepthRange`: stores near/far in the context,
   writes viewport Z scale (`0x44`, `((near+far)/2 - near)` as float >> 8) and Z translate (`0x47`,
   `(near+far)/2 + bias`), then MINZ (`0xd6`) and MAXZ (`0xd7`) with the smaller/larger of the
   two values. */

void GfxDlWriteDepthRange(GuContext *ctx, s32 nearVal, s32 farVal)
{
  u32 maxVal;
  u32 *p;
  float half;
  float scale;
  float trans;

  half = (float)(nearVal + farVal) * 0.5f;
  scale = half - (float)nearVal;
  ctx->depthNear = nearVal;
  ctx->depthFar = farVal;
  maxVal = farVal;
  if (farVal <= nearVal) {
    maxVal = nearVal;
    nearVal = farVal;
  }
  trans = half + (float)ctx->depthBias;
  p = (u32 *)ctx->listCurrent;
  p[0] = (*(u32 *)&scale >> 8) | 0x44000000;
  ctx->listCurrent = (u8 *)(p + 4);
  p[1] = (*(u32 *)&trans >> 8) | 0x47000000;
  p[2] = nearVal | 0xd6000000;
  p[3] = maxVal | 0xd7000000;
}
