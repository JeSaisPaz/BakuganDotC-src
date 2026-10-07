// bdc 0x088a5228 ActorStageObjEggCrystalElementCtor
#include "bdc.h"

/* Constructor of the attribute-coloured egg crystal (vtable `g_actorStageObjEggCrystalElementVtbl`,
   derived from `ActorStageObjEggCrystalCtor`): maps `attr` (clamped to 0..5) through
   `g_eggCrystalAttrElementTable` (`{2,5,4,6,0,3}`) to the element stored in `element`, tints the
   model with that element's colour (`ActorCrystalGetStyleColor`, `GfxModelSetAmbientColor`) when
   it is nonzero, and on stages 0/13 sets darker ambient/diffuse colours. Returns `self`. Called by
   `BtlBakuganRunBallEntry` (the crystal a Bakugan emerges from). */

void *ActorStageObjEggCrystalElementCtor(ActorStageObjEggCrystal *self, float *pos, int attr)
{
  float at[4] __attribute__((aligned(16)));
  float style[4] __attribute__((aligned(16)));
  float colour[4] __attribute__((aligned(16)));
  float tint[4] __attribute__((aligned(16)));
  float fattr;
  int idx;
  int element;

  at[0] = pos[0];
  at[1] = pos[1];
  at[2] = pos[2];
  at[3] = pos[3];
  ActorStageObjEggCrystalCtor(self, at);
  self->base.base.base.vtable = g_actorStageObjEggCrystalElementVtbl;

  fattr = (float)attr;
  if (fattr < 0.0f) {
    idx = 0;
  } else if (fattr <= 5.0f) {
    idx = (int)fattr;
  } else {
    idx = 5;
  }
  element = g_eggCrystalAttrElementTable[idx];
  self->element = element;
  if (element != 0) {
    ActorCrystalGetStyleColor(style, self->element);
    colour[0] = style[0];
    colour[1] = style[1];
    colour[2] = style[2];
    colour[3] = style[3];
    GfxModelSetAmbientColor((GfxModel *)self, colour, NULL);
  }

  if (GameStageIs0Or13() != 0) {
    tint[0] = 0.07f;
    tint[1] = 0.02f;
    tint[2] = 0.17f;
    tint[3] = 1.0f;
    self->base.base.ambient[0] = tint[0];
    self->base.base.ambient[1] = tint[1];
    self->base.base.ambient[2] = tint[2];
    self->base.base.ambient[3] = tint[3];
    tint[0] = 0.45f;
    tint[1] = 0.45f;
    tint[2] = 0.55f;
    tint[3] = 1.0f;
    self->base.base.color[0] = tint[0];
    self->base.base.color[1] = tint[1];
    self->base.base.color[2] = tint[2];
    self->base.base.color[3] = tint[3];
  }
  return self;
}
