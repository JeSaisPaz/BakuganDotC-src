// bdc 0x088e843c ActorNpcSwitchRobotStateShutdown
#include "bdc.h"

/* AI state 8 (slot 43) of the switch robot (`ActorNpcSwitchRobotCtor`, model 0x53, vtable
   `0x08af3d04`), entered when the ball hits its back switch
   (`ActorNpcSwitchRobotCheckSwitchHit`): plays sound 0x2c0003f, motion 0xd and voice 0x27cb; once
   the frame counter passes the motion's end frame turns the jets on (`feetEffects`, sound
   0x2c00040, effect 0x47 at the model matrix, foot effects 0x4a at `footPos[0]`/`footPos[1]`, kept
   on the feet by `ActorNpcSwitchRobotUpdateFeet`), drops a core point gimmick at its position
   (`GameGimmickCorePointCtor`, kind from `g_switchRobotCorePointKinds` indexed by the
   placement's `entryParam45`, record buffer via `GameGimmickCorePointSetBuffer`), increments the
   field HUD counter (`UiFieldHudIncrementCounter`) and plays motion 0xe; then adds 10 to its
   y velocity each frame while below y 1000 and, once the 60-frame timer runs out, pushes its
   switch event `entry` (`GameFieldCharSetPushEvent`). */

void ActorNpcSwitchRobotStateShutdown(ActorNpcSwitchRobot *self)
{
  GfxModel *model = &self->base.base.base;
  GameGimmickRecord *record;
  GameGimmickCorePoint *obj;
  GameGimmickCorePoint *gimmick;
  UiFieldHud *hud;
  GameFieldPlacedChar *placement;
  float scaled[4] __attribute__((aligned(16)));
  float rise[4] __attribute__((aligned(16)));
  s32 ipos[3];
  u32 kindIndex;
  bool fromLow;
  s32 i;

  if (ActorNpcCheckInterrupt(&self->base, 0) != 0) {
    return;
  }
  switch (self->base.subStep) {
  case 0:
    ActorAddSoundEmitter(self, 0x2c0003f, 0, 0);
    ActorPlayMotion(0.2f, self, 0xd, 0, 0);
    self->motionFrame = 0.0f;
    self->motionEnd = GfxModelGetMotionEnd(model);
    ActorPlayVoice(self, 0x27cb);
    self->base.subStep = self->base.subStep + 1;
    break;

  case 1:
    if (GfxModelGetMotionIndex(model) != (u16)self->base.base.motionSlots[0xd]) {
      ActorPlayMotion(0.2f, self, 0xd, 0, 0);
      GfxModelSwapMotionFrame(model, self->motionFrame);
      break;
    }
    self->motionFrame = self->motionFrame + 1.0f;
    if (self->motionFrame <= self->motionEnd) {
      GfxModelSwapMotionFrame(model, self->motionFrame);
      break;
    }
    ActorAddSoundEmitter(self, 0x2c00040, 0, 0);
    self->feetEffects = 1;
    GfxEffectSpawnAtMatrix(g_worldEffectMgr, 0x47, self->base.base.mtx);
    GfxEffectSpawnAttached(g_worldEffectMgr, 0x4a, self->footPos[0]);
    GfxEffectSpawnAttached(g_worldEffectMgr, 0x4a, self->footPos[1]);

    record = MemAllocAligned(sizeof(GameGimmickRecord), true);
    record->heading = 0;
    /* translation row scaled by 0.05 (vscl.t; lane 3 is not used) */
    for (i = 0; i < 3; i++) {
      scaled[i] = self->base.base.mtx[12 + i] * 0.05f;
    }
    for (i = 0; i < 3; i++) {
      if (scaled[i] <= 0.0f) {
        ipos[i] = (s32)(scaled[i] * 4096.0f - 0.5f);
      } else {
        ipos[i] = (s32)(scaled[i] * 4096.0f + 0.5f);
      }
    }
    record->pos[0] = ipos[0];
    record->pos[1] = ipos[1];
    record->pos[2] = ipos[2];
    record->visFlags = record->visFlags & ~0x10;

    placement = (GameFieldPlacedChar *)self->base.base.placement;
    kindIndex = placement->entryParam45;
    if (kindIndex > 2) {
      kindIndex = 0;
    }
    hud = CoreTaskFind(0xbb9);
    if (hud != NULL) {
      UiFieldHudIncrementCounter(hud);
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    obj = MemAlloc(sizeof(GameGimmickCorePoint), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    gimmick = NULL;
    if (obj != NULL) {
      GameGimmickCorePointCtor(obj, g_switchRobotCorePointKinds[kindIndex], record, 0x1778, 0);
      gimmick = obj;
    }
    GameGimmickCorePointSetBuffer(gimmick, record);
    ActorPlayMotion(0.2f, self, 0xe, 1, 0);
    self->timer = 60;
    self->base.subStep = self->base.subStep + 1;
    break;

  case 2:
    if (model->pos[1] < 1000.0f) {
      rise[0] = 0.0f;
      rise[1] = 10.0f;
      rise[2] = 0.0f;
      rise[3] = 0.0f;
      /* velocity.xyz += (0, 10, 0), w untouched (vadd.t) */
      for (i = 0; i < 3; i++) {
        model->velocity[i] = model->velocity[i] + rise[i];
      }
    }
    if (self->timer-- < 0) {
      placement = (GameFieldPlacedChar *)self->base.base.placement;
      GameFieldCharSetPushEvent(g_gameFieldCharSet, placement->entry);
      self->base.subStep = self->base.subStep + 1;
    }
    break;
  }
}
