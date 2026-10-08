// bdc 0x088cad1c UiTalkBalloonDrawFrame
#include "bdc.h"

/* Draws the speech-bubble frame of the talk balloon (`UiTalkBalloonCtor`) into `packet`; returns
   at once when `frameHidden` is set or there is no `frameSprite`. It opens a chunk with the 2D state
   and the screen camera (`g_gfxScreenCamera`), a world matrix scaling x/y by the frame sprite's
   `maybe_sizeW` and translating to the printer origin (`printer->layer.view.w`), then reserves two
   inline vertex arrays (3 floats each) behind JUMPs: 21 border vertices drawn as 17 indexed
   triangles (`g_talkBalloonBorderIndices`) in black at the frame sprite's alpha, and 23 fill
   vertices drawn as 19 indexed triangles (`g_talkBalloonFillIndices`) in white; then, antialiased
   and alpha-blended with the previous frame bound as texture (`GfxDlSetFrameBufferTexture`), line
   strips over fill vertices 1..22 and border vertices 1..20. The chunk is closed before the
   vertices are filled in:
   - half size `hw`/`hh` = `frameSize` × 0.05, edge size `ew`/`eh` = (half − 5) × 0.2 + 5;
   - until `tailSidesLocked`, `tailSides` bit 0 = origin.x < targetPos.x, bit 1 = !(origin.y <=
     targetPos.y); the lock is set once `stateTimer` >= 3. Bit 0 / bit 1 negate hw, ew / hh, eh;
   - border: `g_talkBalloonBorderShape` × (hw, hh), tail points 2..4 as pivot × half + (p −
     pivot) × edge (`g_talkBalloonTailPivot`), pushed out by `g_talkBalloonBorderNormals` ×
     edge × 1.3;
   - tail shift = (target.x − origin.x − vertex 3 x) × 0.4 clamped to ±|hw × 1.5|, added to tail
     points 2..4 of both arrays; base = (fill vertex 3 x, fill vertex 2 y) after the shift;
   - fill: `g_talkBalloonFillShape` the same way, tail points 2..6 also shifted;
   - the target y (relative) is kept at least 20 beyond base y on the tail side; with d = target −
     base, if |d| <= 0.1 it returns, else reach = clamp(|d| − 40, 8, 30) and the direction d/|d|
     has x clamped to ±0.5 and |y| raised to at least 0.4, then is renormalised: fill vertex 4 =
     base + dir × reach, fill vertices 3 and 5 x −= dir.x × reach × 0.5, 2 and 6 x −= dir.x ×
     reach × 0.3; border vertex 3 = base + dir × (reach + |eh| × 3 × ((reach − 8) × 0.02 + 0.5)).
   VFPU: the zero-vector normalise fallback is the bank's S713 (0.0f). */

void UiTalkBalloonDrawFrame(UiTalkBalloon *self, void *packet)
{
  RenderPacket *pkt = packet;
  float colour[4];
  float origin[4];
  union { float f[16]; u32 u[16]; } mat;
  float tail[4];
  float base[4];
  float dir[3];
  float len;
  float lenSq;
  float k;
  u32 *dl;
  u32 *jump;
  float (*border)[3];
  float (*fill)[3];
  float halfW;
  float halfH;
  float edgeW;
  float edgeH;
  float targetX;
  float targetY;
  float shift;
  float lim;
  float reach;
  float reach2;
  s32 row;
  s32 col;
  s32 i;

  if (self->frameHidden != 0 || self->frameSprite == NULL) {
    return;
  }
  dl = GfxPacketBeginChunk(pkt);
  dl = GfxDlCall2DState(dl);
  dl = GfxCameraDlWrite(g_gfxScreenCamera, dl, 0xffffffff);
  origin[0] = self->printer->layer.view.w.x;
  origin[1] = self->printer->layer.view.w.y;
  origin[2] = self->printer->layer.view.w.z;
  origin[3] = self->printer->layer.view.w.w;
  halfW = self->frameSize[0] * 0.05f;
  halfH = self->frameSize[1] * 0.05f;
  for (i = 0; i < 16; i++) {
    mat.f[i] = (i % 5 == 0) ? 1.0f : 0.0f;
  }
  mat.f[0] = self->frameSprite->maybe_sizeW;
  mat.f[5] = self->frameSprite->maybe_sizeW;
  mat.f[10] = 1.0f;
  mat.f[12] = origin[0];
  mat.f[13] = origin[1];
  mat.f[14] = 0.0f;
  mat.f[15] = 0.0f;
  /* WORLD matrix: 0x3a, then 12 data words (x/y/z of each row, float bits >> 8) */
  dl[0] = 0x3a000000;
  for (row = 0; row < 4; row++) {
    for (col = 0; col < 3; col++) {
      dl[1 + row * 3 + col] = (g_talkBalloonWorldDataCmd & 0xff000000) | (mat.u[row * 4 + col] >> 8);
    }
  }
  /* BASE + JUMP over the 21 border vertices */
  jump = dl + 13;
  border = (float (*)[3])(jump + 2);
  dl = (u32 *)&border[21];
  jump[0] = ((PspAddr(dl) >> 0x18) & 0xf) << 0x10 | 0x10000000;
  jump[1] = (PspAddr(dl) & 0xffffff) | 0x8000000;
  colour[0] = 0.0f;
  colour[1] = 0.0f;
  colour[2] = 0.0f;
  colour[3] = self->frameSprite->alpha;
  dl = GfxDlSetBlendState(dl, (const ScePspFVector4 *)colour, 0, 1);
  dl[0] = 0x12000980; /* VTYPE: float position, 8-bit indices */
  dl[1] = ((PspAddr(g_talkBalloonBorderIndices) >> 0x18) & 0xf) << 0x10 | 0x10000000;
  dl[2] = (PspAddr(g_talkBalloonBorderIndices) & 0xffffff) | 0x2000000; /* IADDR */
  dl += 3;
  if (border != NULL) {
    dl[0] = ((PspAddr(border) >> 0x18) & 0xf) << 0x10 | 0x10000000;
    dl[1] = (PspAddr(border) & 0xffffff) | 0x1000000; /* VADDR */
    dl += 2;
  }
  dl[0] = 0x04030033; /* PRIM triangles, 51 indices */
  /* BASE + JUMP over the 23 fill vertices */
  fill = (float (*)[3])(dl + 3);
  jump = (u32 *)&fill[23];
  dl[1] = ((PspAddr(jump) >> 0x18) & 0xf) << 0x10 | 0x10000000;
  dl[2] = (PspAddr(jump) & 0xffffff) | 0x8000000;
  colour[0] = 1.0f;
  colour[1] = 1.0f;
  colour[2] = 1.0f;
  colour[3] = self->frameSprite->alpha;
  dl = GfxDlSetBlendState(jump, (const ScePspFVector4 *)colour, 0, 1);
  dl[0] = 0x12000980;
  dl[1] = ((PspAddr(g_talkBalloonFillIndices) >> 0x18) & 0xf) << 0x10 | 0x10000000;
  dl[2] = (PspAddr(g_talkBalloonFillIndices) & 0xffffff) | 0x2000000;
  dl += 3;
  if (fill != NULL) {
    dl[0] = ((PspAddr(fill) >> 0x18) & 0xf) << 0x10 | 0x10000000;
    dl[1] = (PspAddr(fill) & 0xffffff) | 0x1000000;
    dl += 2;
  }
  dl[0] = 0x04030039; /* PRIM triangles, 57 indices */
  dl[1] = 0x25000001; /* antialiasing on */
  dl[2] = 0xdf000032; /* BLEND src alpha, 1 - src alpha, add */
  dl[3] = 0xe0ffffff; /* SFIX */
  dl[4] = 0xe1000000; /* DFIX */
  dl = GfxDlSetFrameBufferTexture(dl + 5);
  dl[0] = 0x12000180; /* VTYPE: float position */
  dl += 1;
  if (&fill[1] != NULL) {
    dl[0] = ((PspAddr(&fill[1]) >> 0x18) & 0xf) << 0x10 | 0x10000000;
    dl[1] = (PspAddr(&fill[1]) & 0xffffff) | 0x1000000;
    dl += 2;
  }
  dl[0] = 0x04020016; /* PRIM line strip, 22 vertices */
  dl[1] = 0x12000180;
  dl += 2;
  if (&border[1] != NULL) {
    dl[0] = ((PspAddr(&border[1]) >> 0x18) & 0xf) << 0x10 | 0x10000000;
    dl[1] = (PspAddr(&border[1]) & 0xffffff) | 0x1000000;
    dl += 2;
  }
  dl[0] = 0x04020014; /* PRIM line strip, 20 vertices */
  dl[1] = 0xc9000103; /* TFUNC */
  dl[2] = 0x25000000; /* antialiasing off */
  GfxPacketEndChunk(pkt, dl + 3);

  edgeW = (halfW - 5.0f) * 0.2f + 5.0f;
  targetX = self->targetPos[0];
  targetY = self->targetPos[1];
  edgeH = (halfH - 5.0f) * 0.2f + 5.0f;
  if (self->tailSidesLocked == 0) {
    if (origin[0] < targetX) {
      self->tailSides = self->tailSides | 1;
    } else {
      self->tailSides = self->tailSides & ~1u;
    }
    if (origin[1] <= targetY) {
      self->tailSides = self->tailSides & ~2u;
    } else {
      self->tailSides = self->tailSides | 2;
    }
    if (self->stateTimer >= 3) {
      self->tailSidesLocked = 1;
    }
  }
  if (self->tailSides & 1) {
    halfW = -halfW;
    edgeW = -edgeW;
  }
  if (self->tailSides & 2) {
    halfH = -halfH;
    edgeH = -edgeH;
  }

  /* border: base shape, tail scaled around the pivot, then pushed out */
  for (i = 0; i < 21; i++) {
    fill[i][0] = g_talkBalloonBorderShape[i][0] * halfW;
    fill[i][2] = 0.0f;
    fill[i][1] = g_talkBalloonBorderShape[i][1] * halfH;
  }
  for (i = 2; i < 5; i++) {
    fill[i][0] = g_talkBalloonTailPivot[0] * halfW +
                 (g_talkBalloonBorderShape[i][0] - g_talkBalloonTailPivot[0]) * edgeW;
    fill[i][1] = g_talkBalloonTailPivot[1] * halfH +
                 (g_talkBalloonBorderShape[i][1] - g_talkBalloonTailPivot[1]) * edgeH;
  }
  for (i = 0; i < 21; i++) {
    border[i][0] = fill[i][0] + g_talkBalloonBorderNormals[i][0] * edgeW * 1.3f;
    border[i][2] = 0.0f;
    border[i][1] = fill[i][1] + g_talkBalloonBorderNormals[i][1] * edgeH * 1.3f;
  }

  /* tail shift toward the target */
  targetX = targetX - origin[0];
  tail[2] = 0.0f;
  targetY = targetY - origin[1];
  tail[0] = targetX;
  tail[1] = targetY;
  shift = (targetX - fill[3][0]) * 0.4f;
  lim = halfW * 1.5f;
  if (!(-__builtin_fabsf(lim) <= shift)) {
    shift = -__builtin_fabsf(lim);
  } else if (__builtin_fabsf(lim) < shift) {
    shift = __builtin_fabsf(lim);
  }
  for (i = 2; i < 5; i++) {
    fill[i][0] = fill[i][0] + shift;
    border[i][0] = border[i][0] + shift;
  }
  base[0] = fill[3][0];
  base[2] = 0.0f;
  base[1] = fill[2][1];

  /* fill: its own shape, tail scaled around the pivot and shifted */
  for (i = 0; i < 23; i++) {
    fill[i][0] = g_talkBalloonFillShape[i][0] * halfW;
    fill[i][2] = 0.0f;
    fill[i][1] = g_talkBalloonFillShape[i][1] * halfH;
  }
  for (i = 2; i < 7; i++) {
    fill[i][0] = g_talkBalloonTailPivot[0] * halfW +
                 (g_talkBalloonFillShape[i][0] - g_talkBalloonTailPivot[0]) * edgeW;
    fill[i][0] = fill[i][0] + shift;
    fill[i][1] = g_talkBalloonTailPivot[1] * halfH +
                 (g_talkBalloonFillShape[i][1] - g_talkBalloonTailPivot[1]) * edgeH;
  }

  /* keep the target at least 20 beyond the base on the tail side */
  if (self->tailSides & 2) {
    if (!(tail[1] <= base[1] - 20.0f)) {
      tail[1] = base[1] - 20.0f;
    }
  } else if (tail[1] < base[1] + 20.0f) {
    tail[1] = base[1] + 20.0f;
  }

  /* dir = tail - base, len = |dir| */
  dir[0] = tail[0] - base[0];
  dir[1] = tail[1] - base[1];
  dir[2] = tail[2] - base[2];
  len = __builtin_sqrtf(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
  if (len <= 0.1f) {
    return;
  }
  reach = len - 40.0f;
  if (reach < 8.0f) {
    reach = 8.0f;
  } else if (!(reach <= 30.0f)) {
    reach = 30.0f;
  }
  /* dir = normalize(dir), each lane clamped to [-1, 1]; zero length scales by 0 */
  lenSq = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
  k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
  dir[0] = VfSat1(dir[0] * k);
  dir[1] = VfSat1(dir[1] * k);
  dir[2] = VfSat1(dir[2] * k);
  if (!(dir[0] <= 0.5f)) {
    dir[0] = 0.5f;
  } else if (dir[0] < -0.5f) {
    dir[0] = -0.5f;
  }
  if (dir[1] < 0.0f && !(dir[1] <= -0.4f)) {
    dir[1] = -0.4f;
  }
  if (!(dir[1] < 0.0f) && dir[1] < 0.4f) {
    dir[1] = 0.4f;
  }
  /* dir = normalize(dir) */
  lenSq = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
  k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
  dir[0] = VfSat1(dir[0] * k);
  dir[1] = VfSat1(dir[1] * k);
  dir[2] = VfSat1(dir[2] * k);
  fill[4][0] = base[0] + dir[0] * reach;
  fill[4][1] = base[1] + dir[1] * reach;
  fill[3][0] = fill[3][0] - dir[0] * reach * 0.5f;
  fill[5][0] = fill[5][0] - dir[0] * reach * 0.5f;
  fill[2][0] = fill[2][0] - dir[0] * reach * 0.3f;
  fill[6][0] = fill[6][0] - dir[0] * reach * 0.3f;
  reach2 = reach + __builtin_fabsf(edgeH) * 3.0f * ((reach - 8.0f) * 0.02f + 0.5f);
  border[3][0] = base[0] + dir[0] * reach2;
  border[3][1] = base[1] + dir[1] * reach2;
}
