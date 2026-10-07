// bdc 0x088dffa4 ActorSpawn
#include "bdc.h"

/* Factory of the in-world 3D actors that scripts create with `ScriptOpSpawnActor`: allocates the
   actor from the low end of the heap, runs the class constructor selected by `modelId` (falling
   back to 0x2f when the pack chain lacks the model named by `g_btlModelNames`), sets its
   collision layer, snaps/places it at `pos`, scales it by 0.1 and tints it for the current stage.
   Returns the new actor (NULL if the allocation failed; the collider setup after the switch still
   dereferences it). */

void *ActorSpawn(s32 modelId, s32 flag, float *pos)

{
  bool fromLow;
  void *mem;
  Actor *self;
  bool stageLit;
  bool cloak;
  bool toonOk;
  bool isCharacter;

  if (CorePackChainFind(g_ioLzsPackages, g_btlModelNames[modelId]) == NULL) {
    modelId = 0x2f;
  }
  switch (modelId) {
  case 0x2f:
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(ActorPlayer), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
      ActorPlayerCtor((ActorPlayer *)mem, modelId);
    }
    self = (Actor *)mem;
    break;
  case 0x4e:
  case 0x50:
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(ActorNpcGuard), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
      ActorNpcGuardCtor((ActorNpcGuard *)mem, modelId);
    }
    self = (Actor *)mem;
    break;
  case 0x4f:
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(ActorNpcCloak), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
      ActorNpcCloakCtor((ActorNpcCloak *)mem, modelId);
    }
    self = (Actor *)mem;
    break;
  case 0x51:
  case 0x52:
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(ActorNpc), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
      ActorNpcRobotCtor((ActorNpc *)mem, modelId);
    }
    self = (Actor *)mem;
    break;
  case 0x53:
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(ActorNpcSwitchRobot), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
      ActorNpcSwitchRobotCtor((ActorNpcSwitchRobot *)mem, modelId);
    }
    self = (Actor *)mem;
    break;
  default:
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(Actor), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
      ActorCtor((Actor *)mem, modelId);
    }
    self = (Actor *)mem;
    break;
  }

  if (flag == 0) {
    self->isPlayer = 1;
    ActorSetColliderLayer(self, 1);
  }
  else {
    ((CollisionCollider *)self->bodyCollider)->flags |= 0x40;
    ActorSetColliderLayer(self, 5);
  }

  stageLit = false;
  if (modelId >= 0x51 && modelId < 0x54) {
    stageLit = true;
    ActorApplyStageLight(self);
  }
  cloak = (modelId == 0x4f);

  if (pos != NULL) {
    CollisionRaycastPointB(pos, self->base.pos);
    ActorSetHeading(self, pos[3]);
  }

  if (self != NULL) {
    /* scale.xyz *= 0.1f; the sv.q also stores lane 3 from S713, which this function never
       sets (stale VFPU state), so scale.w is left out (see ## Notes) */
    self->base.scale[0] = self->base.scale[0] * 0.1f;
    self->base.scale[1] = self->base.scale[1] * 0.1f;
    self->base.scale[2] = self->base.scale[2] * 0.1f;
  }

  if (!(stageLit | cloak)) {
    toonOk = true;
    if (GameStageIs4To7() != 0) {
      toonOk = false;
    }
    isCharacter = (modelId < 0x30);
    if (GameStageIs0Or13() != 0) {
      self->base.color[0] = 0.45f;
      self->base.color[1] = 0.45f;
      self->base.color[2] = 0.55f;
      self->base.color[3] = 1.0f;
      if (isCharacter) {
        self->base.ambient[3] = 1.0f;
        self->base.ambient[0] = 0.4f;
        self->base.ambient[1] = 0.4f;
        self->base.ambient[2] = 0.4f;
      }
      else {
        self->base.ambient[0] = g_colorWhite.x;
        self->base.ambient[1] = g_colorWhite.y;
        self->base.ambient[2] = g_colorWhite.z;
        self->base.ambient[3] = g_colorWhite.w;
        if (toonOk) {
          GfxModelEnableToon(&self->base, 2);
        }
      }
    }
    else if (!isCharacter && toonOk) {
      GfxModelEnableToon(&self->base, 0);
    }
  }
  return self;
}
