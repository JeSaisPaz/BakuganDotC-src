// bdc 0x088e5a10 ActorNpcCtor
#include "bdc.h"

/* Base constructor of the field NPC/guard classes (models 0x4e..0x53): `ActorCtor`, vtable
   `g_actorNpcVtbl`, freeze kind 0, voice -1, remembers the loaded-package list head
   (`g_ioLzsPackages`) in `packageHead`, clears the AI state (state, phase, sub-step, see result,
   route and stuck counter) and zeroes (the VFPU bank's zero vector C720) the
   route target, noise point, return point, checkpoint and head position; hearing radius 200, view
   distance 70, view half-angle 0.262, speed 1, no head, home heading 0. It then allocates the
   0x28-byte view cone from the low end of the heap (`MemAlloc` under `MemLock`), constructs it
   (`ActorNpcViewConeCtor`, skipped when the allocation fails) and binds it to the effect manager
   of the field's screen-effect holder (`GameFieldScreenFxGetEffects`) and to the actor position
   (`ActorNpcViewConeInit`); turn rate 0.035 (2 degrees), saved state = state, flags cleared.
   Returns `self`. */

ActorNpc *ActorNpcCtor(ActorNpc *self, s32 modelId)

{
  bool fromLow;
  void **fx;
  void *mem;
  void *cone;

  ActorCtor(&self->base, modelId);
  self->base.base.base.vtable = (void *)&g_actorNpcVtbl;
  self->freezeKind = 0;
  self->voiceId = -1;
  self->packageHead = g_ioLzsPackages;
  self->phase = 0;
  self->base.route = NULL;
  self->base.routeStep = 0;
  self->aiState = 0;
  self->subStep = 0;
  self->seeResult = 0;
  /* C720 is the VFPU bank's zero vector. */
  self->base.routeTarget[0] = 0.0f;
  self->base.routeTarget[1] = 0.0f;
  self->base.routeTarget[2] = 0.0f;
  self->base.routeTarget[3] = 0.0f;
  self->noisePoint[0] = 0.0f;
  self->noisePoint[1] = 0.0f;
  self->noisePoint[2] = 0.0f;
  self->noisePoint[3] = 0.0f;
  self->returnPoint[0] = 0.0f;
  self->returnPoint[1] = 0.0f;
  self->returnPoint[2] = 0.0f;
  self->returnPoint[3] = 0.0f;
  self->base.checkpoint[0] = 0.0f;
  self->base.checkpoint[1] = 0.0f;
  self->base.checkpoint[2] = 0.0f;
  self->base.checkpoint[3] = 0.0f;
  self->base.stuckFrames = 0;
  self->hearRadius = 200.0f;
  self->viewDist = 70.0f;
  self->viewHalfAngle = 0.2617994f;
  self->speed = 1.0f;
  self->head = NULL;
  self->headPos[0] = 0.0f;
  self->headPos[1] = 0.0f;
  self->headPos[2] = 0.0f;
  self->headPos[3] = 0.0f;
  self->homeHeading = 0.0f;
  self->forceUpdate = 0;
  fx = ((GameFieldTask *)GameFieldFindTask())->screenFx;
  cone = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x28, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    ActorNpcViewConeCtor(mem);
    cone = mem;
  }
  self->viewCone = cone;
  ActorNpcViewConeInit(cone, GameFieldScreenFxGetEffects(fx), self->base.base.pos);
  self->homeHeading = 0.0f;
  self->turnRate = 0.034906585f;
  self->savedState = self->aiState;
  self->culled = 0;
  self->revealedPlayer = 0;
  self->pauseIdle = 0;
  return self;
}
