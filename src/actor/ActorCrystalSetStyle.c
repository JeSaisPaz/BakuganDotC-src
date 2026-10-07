// bdc 0x08855664 ActorCrystalSetStyle
#include "bdc.h"

/* Applies a colour style to a crystal object (`ActorCrystalCtor`, `ActorCrystalRegenerate`):
   stores `type` and `style`, sets `autoFire` when `type == 2`, takes the attribute id from
   `g_actorCrystalStyleAttributes`, tints the whole model with the style's colour and swaps the
   "inner" texture of the two `mat_context` materials to `fz_crystal01_in_%02d_modulate`. Then clears
   the three crystal slots and, on stages 0 and 13, overrides the ambient and model colours.
   The crystal model `fz_crystal01.gmo` is shown in one of seven colours chosen by `style` (0..6). */

void ActorCrystalSetStyle(ActorCrystal *self, s32 type, s32 style)

{
  float colour[4];
  char textureName[64];
  const float *entry;
  void *texture;
  int i;

  self->type = type;
  self->style = style;
  if (type == 2) {
    self->autoFire = 1;
  }
  self->attribute = g_actorCrystalStyleAttributes[self->style];
  entry = g_actorCrystalStyleColors[self->style];
  colour[0] = entry[0];
  colour[1] = entry[1];
  colour[2] = entry[2];
  colour[3] = entry[3];
  GfxModelSetAmbientColor(&self->base.base, colour, NULL);
  /* word 8 (+0x20) of the 0x30-byte style entry is the integer inner-texture index */
  sprintf(textureName, "fz_crystal01_in_%02d_modulate",
          *(const s32 *)&g_actorCrystalStyleColors[self->style][8]);
  texture = GfxFindTexture(textureName);
  GfxModelSetMaterialTexture(&self->base.base, "mat_context00", texture);
  GfxModelSetMaterialTexture(&self->base.base, "mat_context01", texture);
  for (i = 0; i < 3; i++) {
    self->crystalSlots[i] = NULL;
  }
  if (GameStageIs0Or13() != 0) {
    self->base.base.ambient[0] = 0.07f;
    self->base.base.ambient[1] = 0.02f;
    self->base.base.ambient[2] = 0.17f;
    self->base.base.ambient[3] = 1.0f;
    self->base.base.color[0] = 0.15f;
    self->base.base.color[1] = 0.2f;
    self->base.base.color[2] = 0.2f;
    self->base.base.color[3] = 1.0f;
  }
  return;
}
