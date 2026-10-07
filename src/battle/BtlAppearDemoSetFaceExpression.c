// bdc 0x088ffe30 BtlAppearDemoSetFaceExpression
#include "bdc.h"

/* Face setup of the appear demo for model `model` (nothing when NULL): sets the material depth bias
   (`GfxMaterialState` `depthBias`) of the first materials named `eye_shape_L`/`eye_shape_R` to
   0x1f and of `mayu_L`/`mayu_R` (eyebrows) to 0x40, each found with
   `GfxModelFindMaterialStateBySubstr` and skipped when missing. Reset by
   `BtlAppearDemoClearFaceExpression`; `demo` is unused. */
void BtlAppearDemoSetFaceExpression(void *demo, GfxModel *model)
{
    GfxMaterialState *state;

    (void)demo;
    if (model == NULL) {
        return;
    }
    state = GfxModelFindMaterialStateBySubstr(model, "eye_shape_L");
    if (state != NULL) {
        state->depthBias = 0x1f;
    }
    state = GfxModelFindMaterialStateBySubstr(model, "eye_shape_R");
    if (state != NULL) {
        state->depthBias = 0x1f;
    }
    state = GfxModelFindMaterialStateBySubstr(model, "mayu_L");
    if (state != NULL) {
        state->depthBias = 0x40;
    }
    state = GfxModelFindMaterialStateBySubstr(model, "mayu_R");
    if (state != NULL) {
        state->depthBias = 0x40;
    }
}
