// bdc 0x0890b000 UiLoadingDrawPropellers
#include "bdc.h"

/* Draw step of the propeller themes of the now-loading screen (task 10100 / 0x2774,
   `UiLoadingCtor`, called from `UiLoadingDraw`; shared objects `g_uiLoadingShared` from
   `UiLoadingInitShared`): opens a 2D render packet at depth 1500 and draws the background bands
   (`GfxPacketDrawGradientBands`, rows `g_uiLoadingBandYs` / `g_uiLoadingBandColours`: variant 1
   for theme 0x12, variant 2 with 4 bands for theme 0x13, else variant 0), grows sprite 8 by
   `frameScale` and pulses sprite 9's alpha. Then it steps the flying body (sprite 1) by
   `propPhase`: 0 = bob `propPos` with `propVel`/`propAccel` and tilt with the stick (`propSpin`,
   clamped to ±π/8, drives `propRot[1]`), 1 = rise for 6 frames then go to 2 with `propVel[0]` -8,
   2 = fly off and shrink; any other phase leaves it. The body sits at `scale + propPos + propRot`
   (`+0x90`, keeping `posZ`); sprites 2..5 (pulsing every 6 frames, `frameCounter`) and 17..20
   (texture `"propeller_%02d"` with `propFrame / 2`, `propFrame` cycling 0..5) are placed around it,
   their offset from the body rotated by the body's angle and scaled by its X scale.
   The 4th lane of every position written is the body's `angle` (`+0x9c`); `posZ` is kept. */

void UiLoadingDrawPropellers(UiLoading *self)
{
  s32 row;
  s32 count;
  s32 i;
  void *packet;
  float scale;
  float stick;
  float spin;
  float atan;
  float angle;
  float dist;
  float mul;
  float dx;
  float dy;
  float dz;
  float offX;
  float offY;
  GfxSprite *body;
  GfxSprite *sprite;
  char name[64];

  row = 0;
  count = 1;
  if (self->theme == 0x12) {
    row = 1;
  } else if (self->theme == 0x13) {
    row = 2;
    count = 4;
  }
  packet = GfxNewRenderPacket(1500.0f);
  GfxPacketCall2DState(packet);
  GfxPacketDrawGradientBands(packet, g_uiLoadingBandYs[row], g_uiLoadingBandColours[row], count, 1);
  GfxSpriteGetWidth(g_uiLoadingShared->sprites[8]);
  GfxSpriteGetHeight(g_uiLoadingShared->sprites[8]);
  self->frameScale = self->frameScale + 0.005f;
  scale = self->frameScale * 0.2f + 1.0f;
  GfxSpriteSetScaleRotation(g_uiLoadingShared->sprites[8], scale, scale, 0.0f, false);
  g_uiLoadingShared->sprites[9]->alpha = (1.0f - __builtin_cosf(self->frameScale * 12.0f * 3.1415927f)) * 0.5f * 0.4f + 0.6f;
  body = g_uiLoadingShared->sprites[1];

  if (self->propPhase == 0) {
    if (self->propAccel[0] < 0.0f) {
      if (!(self->propVel[0] < -0.5f)) {
        self->propVel[0] = self->propVel[0] + self->propAccel[0];
      }
      self->propPos[0] = self->propPos[0] + self->propVel[0];
      if (self->propPos[0] <= -40.0f) {
        self->propAccel[0] = -self->propAccel[0];
      }
      if (!(body->maybe_billboardParams80[2] <= -0.06283186f)) {
        body->maybe_billboardParams80[2] = body->maybe_billboardParams80[2] - 0.0015707965f;
      }
    } else {
      if (self->propVel[0] <= 0.5f) {
        self->propVel[0] = self->propVel[0] + self->propAccel[0];
      }
      self->propPos[0] = self->propPos[0] + self->propVel[0];
      if (!(self->propPos[0] < 60.0f)) {
        self->propAccel[0] = -self->propAccel[0];
      }
      if (body->maybe_billboardParams80[2] < 0.06283186f) {
        body->maybe_billboardParams80[2] = body->maybe_billboardParams80[2] + 0.0015707965f;
      }
    }
    if (self->propAccel[1] < 0.0f) {
      if (!(self->propVel[1] < -0.2f)) {
        self->propVel[1] = self->propVel[1] + self->propAccel[1];
      }
      self->propPos[1] = self->propPos[1] + self->propVel[1];
      if (self->propPos[1] <= -10.0f) {
        self->propAccel[1] = -self->propAccel[1];
      }
    } else {
      if (self->propVel[1] <= 0.2f) {
        self->propVel[1] = self->propVel[1] + self->propAccel[1];
      }
      self->propPos[1] = self->propPos[1] + self->propVel[1];
      if (!(self->propPos[1] < 5.0f)) {
        self->propAccel[1] = -self->propAccel[1];
      }
    }
    /* stick Y with a 0.2 dead zone */
    stick = g_padState->stickY;
    if (stick < 0.0f) {
      stick = stick + 0.2f;
      if (!(stick <= 0.0f)) {
        stick = 0.0f;
      }
    }
    if (!(stick <= 0.0f)) {
      stick = stick - 0.2f;
      if (stick < 0.0f) {
        stick = 0.0f;
      }
    }
    spin = self->propSpin - stick * 0.003f;
    self->propSpin = spin;
    if (spin < -0.3926991f) {
      spin = -0.3926991f;
    } else if (!(spin <= 0.3926991f)) {
      spin = 0.3926991f;
    }
    self->propSpin = spin;
    self->propRot[1] = __builtin_cosf(spin * 3.1415927f + 1.5707964f) * 50.0f;
  } else if (self->propPhase == 1) {
    if (body->maybe_billboardParams80[2] != 0.0f) {
      body->maybe_billboardParams80[2] = body->maybe_billboardParams80[2] * 0.3f;
    }
    self->propTimer = self->propTimer + 1;
    self->propPos[1] = self->propPos[1] + 4.0f;
    if (!(self->propTimer < 6)) {
      self->propVel[0] = -8.0f;
      self->propPhase = self->propPhase + 1;
    }
  } else if (self->propPhase == 2) {
    if (!(self->propPos[1] <= -70.0f)) {
      self->propPos[1] = self->propPos[1] - 8.0f;
    }
    if (self->propVel[0] < 0.0f) {
      if (self->propPos[0] < -50.0f) {
        self->propVel[0] = 8.0f;
      }
    }
    self->propPos[0] = self->propPos[0] + self->propVel[0];
    body->maybe_billboardParams80[0] = body->maybe_billboardParams80[0] - 0.05f;
    body->maybe_billboardParams80[1] = body->maybe_billboardParams80[1] - 0.05f;
    if (body->maybe_billboardParams80[0] < 0.0f) {
      body->maybe_billboardParams80[0] = 0.0f;
      body->maybe_billboardParams80[1] = 0.0f;
    }
  }
  self->propSpin = self->propSpin * 0.99f;

  /* body pos = base (+0x90) + propPos + propRot in x/y, pos.w = base.w (angle); posZ kept */
  body->posX = body->scaleX + self->propPos[0] + self->propRot[0];
  body->posY = body->scaleY + self->propPos[1] + self->propRot[1];
  body->posW = body->angle;
  GfxSpriteSetScaleRotation(body, body->maybe_billboardParams80[0], body->maybe_billboardParams80[1],
                            body->maybe_billboardParams80[2], false);

  /* sprites 2..5: pulse their scale, then place them around the body */
  i = 0;
  do {
    sprite = g_uiLoadingShared->sprites[i + 2];
    if (self->frameCounter < 6) {
      sprite->maybe_billboardParams80[0] = sprite->maybe_billboardParams80[0] + 0.05f;
      sprite->maybe_billboardParams80[1] = sprite->maybe_billboardParams80[1] - 0.02f;
    } else {
      sprite->maybe_billboardParams80[0] = sprite->maybe_billboardParams80[0] - 0.05f;
      sprite->maybe_billboardParams80[1] = sprite->maybe_billboardParams80[1] + 0.02f;
    }
    GfxSpriteSetScaleRotation(sprite, sprite->maybe_billboardParams80[0] * body->maybe_billboardParams80[0],
                              sprite->maybe_billboardParams80[1] * body->maybe_billboardParams80[1],
                              sprite->maybe_billboardParams80[2] + body->maybe_billboardParams80[2],
                              false);
    /* diff = sprite base - body base, dist = |diff.xyz| */
    dx = sprite->scaleX - body->scaleX;
    dy = sprite->scaleY - body->scaleY;
    dz = sprite->scaleZ - body->scaleZ;
    dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    atan = atan2f(dy, dx);
    /* off = (cos, sin) of (body angle + atan) * dist; pos = body base + propPos + propRot
       + off * body X scale, pos.w = body angle; posZ kept */
    angle = body->maybe_billboardParams80[2] + atan;
    offX = __builtin_cosf(angle) * dist;
    offY = __builtin_sinf(angle) * dist;
    mul = body->maybe_billboardParams80[0];
    sprite->posX = body->scaleX + self->propPos[0] + self->propRot[0] + offX * mul;
    sprite->posY = body->scaleY + self->propPos[1] + self->propRot[1] + offY * mul;
    sprite->posW = body->angle;
    i++;
  } while (i < 4);

  self->frameCounter = self->frameCounter + 1;
  if (!(self->frameCounter < 12)) {
    self->frameCounter = 0;
  }
  sprintf(name, "propeller_%02d", self->propFrame / 2);
  self->propFrame = (self->propFrame + 1) % 6;

  /* sprites 17..20: the propellers, placed like sprites 2..5 and given the current frame */
  i = 0;
  do {
    sprite = g_uiLoadingShared->sprites[i + 17];
    GfxSpriteSetScaleRotation(sprite, sprite->maybe_billboardParams80[0] * body->maybe_billboardParams80[0],
                              sprite->maybe_billboardParams80[1] * body->maybe_billboardParams80[1],
                              sprite->maybe_billboardParams80[2] + body->maybe_billboardParams80[2],
                              false);
    /* diff = sprite base - body base, dist = |diff.xyz| */
    dx = sprite->scaleX - body->scaleX;
    dy = sprite->scaleY - body->scaleY;
    dz = sprite->scaleZ - body->scaleZ;
    dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    atan = atan2f(dy, dx);
    /* off = (cos, sin) of (body angle + atan) * dist; pos = body base + propPos + propRot
       + off * body X scale, pos.w = body angle; posZ kept */
    angle = body->maybe_billboardParams80[2] + atan;
    offX = __builtin_cosf(angle) * dist;
    offY = __builtin_sinf(angle) * dist;
    mul = body->maybe_billboardParams80[0];
    sprite->posX = body->scaleX + self->propPos[0] + self->propRot[0] + offX * mul;
    sprite->posY = body->scaleY + self->propPos[1] + self->propRot[1] + offY * mul;
    sprite->posW = body->angle;
    sprite->texture = GfxFindTexture(name);
    i++;
  } while (i < 4);
}
