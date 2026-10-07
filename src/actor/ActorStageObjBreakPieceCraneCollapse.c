// bdc 0x088b013c ActorStageObjBreakPieceCraneCollapse
#include "bdc.h"

/* Piece handler 3 (`f0_break_crane01`, table `0x08a84c3c`): spawns the dust effect 4 at the base,
   plays the falling motion while tracking the `break_head` node (`GfxModelGetNodeWorldPos`) and,
   when the head hits the ground (radius-10 sphere `g_collisionSweptSphereDesc` swept 10 units
   along `g_vecDown`, `CollisionRaycast`), spawns impact effects 9/10 and sound 0x20000d
   (falling sound 0x2000e7 at frame 40); then frees the motion, waits 90 frames, blinks, fades and
   deletes itself like `ActorStageObjBreakPieceCollapse`. While invisible it also releases its
   owned type-4 effect sprites. The sweep direction's w lane keeps a stale VFPU lane in the
   original and is not written here. */

void ActorStageObjBreakPieceCraneCollapse(ActorStageObjBreakPiece *self)

{
  float basePos[4] __attribute__((aligned(16)));
  ScePspFVector4 nodePos __attribute__((aligned(16)));
  float scale[4] __attribute__((aligned(16)));
  float head[4] __attribute__((aligned(16)));
  float ground[4] __attribute__((aligned(16)));
  GfxEffect *effect;
  GfxEffect *next;
  float *bounds;
  float y;
  float alpha;
  GmoModel *data;

  if (self->hidden != 0 && self->base.base.ambient[3] <= 0.0f) {
    for (effect = (GfxEffect *)g_btlUnitEffectMgr->base.head; effect != NULL; effect = next) {
      next = (GfxEffect *)effect->base.next;
      if (effect->id == 4 && effect->ownerBakugan == (void *)self) {
        UiSpriteLayerRelease(effect->mgr, effect);
      }
    }
  }
  switch (self->step) {
  case 0:
    basePos[0] = self->base.base.pos[0];
    basePos[1] = self->base.base.pos[1];
    basePos[2] = self->base.base.pos[2];
    basePos[3] = self->base.base.pos[3];
    y = basePos[1];
    bounds = ActorStageObjGetBounds(&self->base);
    basePos[1] = y + bounds[1];
    GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 4, basePos, self);
    self->timer = 0;
    self->step = self->step + 1;
    /* fallthrough */
  case 1:
    if (self->timer == 40 && SndHasListener()) {
      SndListener *listener = SndGetListener();
      SndEmitterCreateAtPos(listener, 0x2000e7, &self->base.base.data->rootMatrix[12], 0, 1);
    }
    self->timer = self->timer + 1;
    GfxModelUpdateAndApplyMotion(&self->base.base);
    GfxModelGetNodeWorldPos(&self->base.base, &nodePos, "break_head");
    head[0] = nodePos.x;
    head[1] = nodePos.y;
    head[2] = nodePos.z;
    head[3] = nodePos.w;
    if (self->base.base.motionEnded != 0 || GfxModelMotionReached(&self->base.base, 0.8f)) {
      self->step = self->step + 1;
    }
    g_collisionSweptSphereDesc.start.x = head[0];
    g_collisionSweptSphereDesc.start.y = head[1];
    g_collisionSweptSphereDesc.start.z = head[2];
    g_collisionSweptSphereDesc.start.w = head[3];
    scale[2] = 10.0f;
    scale[1] = 10.0f;
    scale[0] = 10.0f;
    scale[3] = 0.0f;
    /* dir = g_vecDown * scale (vmul.t, staged through nodePos); lane w is a stale VFPU
       lane (S713) the function never sets, so it is left out. */
    nodePos.x = g_vecDown.x * scale[0];
    nodePos.y = g_vecDown.y * scale[1];
    nodePos.z = g_vecDown.z * scale[2];
    g_collisionSweptSphereDesc.dir.x = nodePos.x;
    g_collisionSweptSphereDesc.dir.y = nodePos.y;
    g_collisionSweptSphereDesc.dir.z = nodePos.z;
    g_collisionSweptSphereDesc.radius = 10.0f;
    g_collisionSweptSphereDesc.start.w = 10.0f * 10.0f;
    if (CollisionRaycast(0x31bf337e, g_collisionSweptSphereDesc.shapeBlock, 2) != NULL) {
      GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 9, head, self);
      if (SndHasListener()) {
        SndEmitterCreateAtPos(SndGetListener(), 0x20000d, head, 0, 1);
      }
      ground[0] = head[0];
      ground[1] = head[1];
      ground[2] = head[2];
      ground[3] = head[3];
      ground[1] = BtlStageGetWaterBedHeight();
      GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 10, ground, self);
    }
    break;
  case 2:
    data = self->base.base.data;
    self->timer = 90;
    if (data->motions != NULL) {
      GmoMotionArrayRelease((short *)self->base.base.data->motions,
                            self->base.base.data->motionCount);
    }
    self->step = self->step + 1;
    /* fallthrough */
  case 3:
    if (self->timer == 0) {
      self->step = self->step + 1;
    } else {
      self->timer = self->timer - 1;
    }
    break;
  case 4:
    if (self->timer == 32) {
      self->baseAlpha = 1.0f;
      self->step = self->step + 1;
    } else {
      self->baseAlpha = (self->timer & 2) ? 1.0f : 0.0f;
      self->timer = self->timer + 1;
    }
    break;
  case 5:
    alpha = self->baseAlpha - 0.05f;
    self->baseAlpha = alpha;
    if (alpha <= 0.0f) {
      self->baseAlpha = 0.0f;
      self->step = self->step + 1;
    }
    break;
  case 6:
    CoreObjectDeferDelete(&self->base.base.base, 0);
    self->step = self->step + 1;
    break;
  }
}
