// bdc 0x088e8934 ActorNpcRobotSetupModel
#include "bdc.h"

/* Installs the robot material callback `0x088e8908` (`GfxModelForEachMaterial`) and sets the shadow
   sub-object (`+0x16c`) size to 25. */

void ActorNpcRobotSetupModel(ActorNpc *self)

{
  float *shadow;

  GfxModelForEachMaterial((GfxModel *)self, ActorNpcRobotModelMaterialCallback, (void *)0x0);
  shadow = (float *)self->base.shadow;
  shadow[7] = 0.0f;
  shadow[4] = 25.0f;
  shadow[5] = 25.0f;
  shadow[6] = 25.0f;
}
