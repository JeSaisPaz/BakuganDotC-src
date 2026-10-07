// bdc 0x08855384 ActorCrystalInitMaterials
#include "bdc.h"

/* Sets up the animated materials of a crystal model: installs per-frame update callbacks
   (`ActorCrystalMaterialWriteScrollV`, `GfxDlWriteTexOffsetUAndSpecular`) with their parameter
   blocks at `+0x800..+0x83f` on the materials `mat_context00`, `mat_context01`, `mat_spel` and
   `mat_Ambient`, and sets the blend/lighting flag bits of `mat_context00/01`, `mat_spel`,
   `mat_Ambient` and `mat_Speculer`. Called once by `ActorCrystalCtor`. */

void ActorCrystalInitMaterials(ActorCrystal *self)
{
  float *scroll;
  GfxMaterialState *mat;

  /* four {pos, speed, 0, 0} scrollers (see ActorCrystalUvScrollStep) */
  scroll = (float *)&self->uvScrollers[0x00];
  scroll[0] = 0.0f;
  scroll[1] = 1.0f / 180.0f;
  scroll[2] = 0.0f;
  scroll[3] = 0.0f;
  GfxModelSetMaterialAnimCallback((GfxModel *)self, "mat_context00",
                                  ActorCrystalMaterialWriteScrollV, scroll);
  scroll = (float *)&self->uvScrollers[0x10];
  scroll[0] = 0.0f;
  scroll[1] = 1.0f / 180.0f;
  scroll[2] = 0.0f;
  scroll[3] = 0.0f;
  GfxModelSetMaterialAnimCallback((GfxModel *)self, "mat_context01",
                                  ActorCrystalMaterialWriteScrollV, scroll);
  scroll = (float *)&self->uvScrollers[0x20];
  scroll[0] = 0.0f;
  scroll[1] = 1.0f / 240.0f;
  scroll[2] = 0.0f;
  scroll[3] = 0.0f;
  GfxModelSetMaterialAnimCallback((GfxModel *)self, "mat_spel",
                                  ActorCrystalMaterialWriteScrollV, scroll);
  scroll = (float *)&self->uvScrollers[0x30];
  scroll[0] = 0.0f;
  scroll[1] = 0.005f;
  scroll[2] = 0.0f;
  scroll[3] = 0.0f;
  GfxModelSetMaterialAnimCallback((GfxModel *)self, "mat_Ambient",
                                  GfxDlWriteTexOffsetUAndSpecular, scroll);
  GfxModelSetMaterialAnimCallback((GfxModel *)self, "mat_Ambient",
                                  ActorCrystalMaterialWriteScrollV, scroll);

  if (self->additive == 0) {
    mat = GfxModelFindMaterialStateBySubstr((GfxModel *)self, "mat_context00");
    if (mat != NULL) {
      mat->renderFlags = (mat->renderFlags & 0xf3) | 0x08;
    }
    mat = GfxModelFindMaterialStateBySubstr((GfxModel *)self, "mat_context01");
    if (mat != NULL) {
      mat->renderFlags = (mat->renderFlags & 0xf3) | 0x08;
    }
    mat = GfxModelFindMaterialStateBySubstr((GfxModel *)self, "mat_spel");
    if (mat != NULL) {
      mat->renderFlags = (mat->renderFlags & 0xf3) | 0x08;
    }
  } else {
    GfxModelForEachMaterial((GfxModel *)self, ActorCrystalMaterialSetAdditive, NULL);
  }

  mat = GfxModelFindMaterialStateBySubstr((GfxModel *)self, "mat_Ambient");
  if (mat != NULL) {
    mat->renderFlags = (mat->renderFlags & 0x3f) | 0x40;
    mat->shadeFlags = (mat->shadeFlags & 0x1f) | 0xc0;
    mat->renderFlags = (mat->renderFlags & 0xfc) | 0x02;
  }
  mat = GfxModelFindMaterialStateBySubstr((GfxModel *)self, "mat_Speculer");
  if (mat != NULL) {
    mat->renderFlags = (mat->renderFlags & 0x3f) | 0x80;
  }
}
