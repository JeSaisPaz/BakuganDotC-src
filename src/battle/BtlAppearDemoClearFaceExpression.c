// bdc 0x088ffee0 BtlAppearDemoClearFaceExpression
#include "bdc.h"

/* Resets the face materials of `model` after the appear demo: sets the byte
   at +6 (depthBias) of the material states whose names contain eye_shape_L,
   eye_shape_R, mayu_L and mayu_R to 0. Does nothing for a NULL model. */
void BtlAppearDemoClearFaceExpression(void *demo, void *model)
{
    GfxMaterialState *state;

    (void)demo;
    if (model == NULL) {
        return;
    }
    state = GfxModelFindMaterialStateBySubstr(model, "eye_shape_L");
    if (state != NULL) {
        state->depthBias = 0;
    }
    state = GfxModelFindMaterialStateBySubstr(model, "eye_shape_R");
    if (state != NULL) {
        state->depthBias = 0;
    }
    state = GfxModelFindMaterialStateBySubstr(model, "mayu_L");
    if (state != NULL) {
        state->depthBias = 0;
    }
    state = GfxModelFindMaterialStateBySubstr(model, "mayu_R");
    if (state != NULL) {
        state->depthBias = 0;
    }
}
