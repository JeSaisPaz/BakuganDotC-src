// bdc 0x088a3190 ActorStageObjLandmarkState01Broken
#include "bdc.h"

/* State 1 of the landmark (after `ActorStageObjLandmarkBreak`): each frame spins the orb
   (`orbAngle` += 0.03 rad, wrapped into (-pi, pi]), writes the Y rotation by that angle into the
   orb node's localMatrix rows 0..2 and rebuilds the model matrix (`ActorStageObjUpdateTransform`,
   `ActorStageObjStepUvScrolls`). Step 0 marks the object `dead`; steps 0/1 keep it invisible
   (fade 0, draw alpha 0) and, since that alpha is below 0.3, flag the companion unit dead
   (`combat.dead`), mark the layout spawn record done and release it
   (`ActorStageObjReleaseRecord`) and start a 60-frame timer. Step 2 counts it down. Step 3 makes
   the object opaque again, repeats the unit/record release, sets model colour (0,0,0,1), ambient
   (1, 0.35, 0.157, 0.7) and material ambient (0.45, 0.65, 0.85, 1)
   (`GfxModelSetAmbientColor`) and spawns the broken aura
   (`ActorStageObjLandmarkSpawnBrokenAura`). */

void ActorStageObjLandmarkState01Broken(ActorStageObjLandmark *self)
{
  float tmp[4];
  float colour[4];
  float alpha;
  float c;
  float s;
  float *m;
  s32 step;
  s32 timer;

  self->orbAngle = self->orbAngle + 0.0299999993f;
  if (!(self->orbAngle <= 3.14159274f)) {
    self->orbAngle = self->orbAngle - 6.28318548f;
  } else if (self->orbAngle <= -3.14159274f) {
    self->orbAngle = self->orbAngle + 6.28318548f;
  }
  /* The binary first multiplies an uninitialised stack matrix by Rx and Rz and then overwrites it
     with the plain Y rotation (same as ActorStageObjLandmarkState00Active): only Ry survives. vrot
     with angle * S703 (2/pi) in quarter turns is cos/sin of the angle in radians. */
  c = __builtin_cosf(self->orbAngle);
  s = __builtin_sinf(self->orbAngle);
  if (self->orbNode != NULL) {
    m = self->orbNode->localMatrix;
    m[0] = c;
    m[1] = 0.0f;
    m[2] = -s;
    m[3] = 0.0f;
    m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = s;
    m[9] = 0.0f;
    m[10] = c;
    m[11] = 0.0f;
  }
  ActorStageObjUpdateTransform(&self->base);
  ActorStageObjStepUvScrolls(&self->base);

  step = self->step;
  if (step < 2) {
    if (step < 0) {
      return;
    }
    if (step <= 0) {
      self->base.dead = 1;
      self->step = self->step + 1;
    }
    self->base.fade = 0.0f;
    alpha = self->base.baseAlpha * 0.0f;
    self->base.base.ambient[3] = alpha;
    if (alpha < 0.3f) {
      if (self->unit != NULL) {
        self->unit->base.combat.dead = 1;
      }
      if (self->base.record != NULL) {
        ((ActorStageObjRecord *)self->base.record)->doneFlags[0] = 1;
        ActorStageObjReleaseRecord(self->base.record);
        self->base.record = NULL;
      }
      self->step = self->step + 1;
      self->timer = 60;
    }
  } else if (step < 3) {
    timer = self->timer - 1;
    self->timer = timer;
    if (timer <= 0) {
      self->step = self->step + 1;
    }
  } else if (step < 4) {
    self->base.base.ambient[3] = 1.0f;
    self->base.fade = 1.0f;
    if (self->unit != NULL) {
      self->unit->base.combat.dead = 1;
    }
    if (self->base.record != NULL) {
      ((ActorStageObjRecord *)self->base.record)->doneFlags[0] = 1;
      ActorStageObjReleaseRecord(self->base.record);
      self->base.record = NULL;
    }
    /* model colour = (0, 0, 0, 1), then ambient = (1, 0.35, 0.157, 0.7), each staged in a stack
       temp. */
    tmp[0] = 0.0f;
    tmp[1] = 0.0f;
    tmp[2] = 0.0f;
    tmp[3] = 1.0f;
    self->base.base.color[0] = tmp[0];
    self->base.base.color[1] = tmp[1];
    self->base.base.color[2] = tmp[2];
    self->base.base.color[3] = tmp[3];
    tmp[0] = 1.0f;
    tmp[1] = 0.35f;
    tmp[2] = 0.157f;
    tmp[3] = 0.7f;
    self->base.base.ambient[0] = tmp[0];
    self->base.base.ambient[1] = tmp[1];
    self->base.base.ambient[2] = tmp[2];
    self->base.base.ambient[3] = tmp[3];
    colour[0] = 0.45f;
    colour[1] = 0.65f;
    colour[2] = 0.85f;
    colour[3] = 1.0f;
    GfxModelSetAmbientColor(&self->base.base, colour, NULL);
    ActorStageObjLandmarkSpawnBrokenAura(self);
    self->step = self->step + 1;
  }
}
