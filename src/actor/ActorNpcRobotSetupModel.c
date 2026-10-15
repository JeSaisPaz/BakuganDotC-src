// bdc 0x088e8934 ActorNpcRobotSetupModel
#include "bdc.h"

/* Installs the robot material callback `0x088e8908` (`GfxModelForEachMaterial`) and sets the shadow
   sub-object (`+0x16c`) size to 25. */

void ActorNpcRobotSetupModel(ActorNpc *self)

{
  BtlShadow *shadow;

  GfxModelForEachMaterial((GfxModel *)self, ActorNpcRobotModelMaterialCallback, (void *)0x0);
  shadow = (BtlShadow *)self->base.shadow;
  shadow->size[3] = 0.0f;
  shadow->size[0] = 25.0f;
  shadow->size[1] = 25.0f;
  shadow->size[2] = 25.0f;
}
