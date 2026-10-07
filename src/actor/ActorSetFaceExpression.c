// bdc 0x088df65c ActorSetFaceExpression
#include "bdc.h"

/* Sets the face expression `expr` (`+0x358`) of a character model: binds the five face nodes
   `eye_shape_L/R__A3_BR`, `mayu_L/R__A3_BR_DE` and `mouth__BR` to the node callback `0x088dbf94`
   with the UV offsets of row `expr` of the table `0x08a98580` (0x28 bytes per expression, 8 per
   node). */

void ActorSetFaceExpression(Actor *self, u32 expr)

{
  const char *names[5] = {
      "eye_shape_L__A3_BR", "eye_shape_R__A3_BR", "mayu_L__A3_BR_DE", "mayu_R__A3_BR_DE", "mouth__BR",
  };
  int i;

  self->faceExpression = expr & 0xff;
  for (i = 0; i < 5; i++) {
    GfxModelSetMaterialAnimCallback(&self->base, names[i], ActorFaceNodeWriteUvOffset,
                                    (void *)g_actorFaceUvTable[self->faceExpression][i]);
  }
}
