// bdc 0x088a9ae4 ActorStageObjState05Update
#include "bdc.h"

/* State 5 handler of the shared stage-object state machine (state `+0x304`, `MemberFnPtr` table
   `0x08a842f8`, run by `ActorStageObjUpdate`): destruction flash. Step 0 spawns unit effect 2
   above the object (y + bounds min y, `GfxEffectSpawnWithOwner` on `g_btlUnitEffectMgr`), copies
   the model colour into `tint`, sets `emissive` to white and starts a 110-frame timer. Step 1 ramps
   emissive (+0.02 red, -0.02 green/blue) and tint (+0.01 / -0.01) each frame, writes tint back to
   the model colour and applies emissive as ambient (`GfxModelSetAmbientColor`); when the timer
   is 0 it sets collider flag bit 1, enables lighting, makes every material translucent
   (`ActorStageObjMaterialSetTranslucent`) and spawns effect 1. Step 2 fades out by 0.2 per frame
   and, once faded, leaves remains (`ActorStageObjSpawnRemains`). Step 3 drops an item
   (`ActorStageObjDropItem`) and requests removal. */

void ActorStageObjState05Update(ActorStageObjBase *self)
{
  float pos[4];
  float *bounds;
  float y;
  float fade;
  s32 step;
  s32 timer;

  step = self->step;
  if (step < 2) {
    if (step < 0) {
      return;
    }
    if (step <= 0) {
      pos[0] = self->base.pos[0];
      pos[1] = self->base.pos[1];
      pos[2] = self->base.pos[2];
      pos[3] = self->base.pos[3];
      y = pos[1];
      bounds = ActorStageObjGetBounds(self);
      pos[1] = y + bounds[1];
      GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 2, pos, self);
      self->tint[0] = self->base.color[0];
      self->tint[1] = self->base.color[1];
      self->tint[2] = self->base.color[2];
      self->tint[3] = self->base.color[3];
      self->emissive[0] = 1.0f;
      self->emissive[1] = 1.0f;
      self->emissive[2] = 1.0f;
      self->emissive[3] = 1.0f;
      self->timer = 110;
      self->step = self->step + 1;
    }
    timer = self->timer;
    if (timer == 0) {
      ((CollisionCollider *)self->collider)->flags |= 2;
      self->base.lighting = 1;
      GfxModelForEachMaterial(&self->base, ActorStageObjMaterialSetTranslucent, NULL);
      pos[0] = self->base.pos[0];
      pos[1] = self->base.pos[1];
      pos[2] = self->base.pos[2];
      pos[3] = self->base.pos[3];
      y = pos[1];
      bounds = ActorStageObjGetBounds(self);
      pos[1] = y + bounds[1];
      GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 1, pos, self);
      self->step = self->step + 1;
    } else {
      self->emissive[0] = self->emissive[0] + 0.02f;
      self->emissive[1] = self->emissive[1] - 0.02f;
      self->emissive[2] = self->emissive[2] - 0.02f;
      self->tint[0] = self->tint[0] + 0.01f;
      self->tint[1] = self->tint[1] - 0.01f;
      self->tint[2] = self->tint[2] - 0.01f;
      self->base.color[0] = self->tint[0];
      self->base.color[1] = self->tint[1];
      self->base.color[2] = self->tint[2];
      self->base.color[3] = self->tint[3];
      GfxModelSetAmbientColor(&self->base, self->emissive, NULL);
      self->timer = self->timer - 1;
    }
  } else if (step < 3) {
    fade = self->fade - 0.2f;
    self->fade = fade;
    if (fade <= 0.0f) {
      self->fade = 0.0f;
      ActorStageObjSpawnRemains(self);
      self->step = self->step + 1;
    }
  } else if (step < 4) {
    ActorStageObjDropItem(self);
    self->removeRequest = 1;
  }
}
