// bdc 0x0889e298 BtlStageSetupUvAnim
#include "bdc.h"

/* Registers UV animation slot `slot` (0..3): stores the animation `type` in
   `g_btlStageUvAnimTypes` and its parameter block `params` (scroll speeds, in the map model) in
   `g_btlStageUvAnimParams`, and installs the matching material callback (types 1..4:
   `BtlStageUvType1TexOffsetUCallback`, `BtlStageUvType2TexOffsetUCallback`,
   `BtlStageUvType3TexOffsetUCallback`, `BtlStageUvType4TexOffsetVCallback`) on material
   `material` of arena model `g_btlArenaModels``[model]` via `GfxModelSetMaterialAnimCallback`;
   other types install nothing. Stepped per frame by `BtlStageUpdateUvAnims`. */

void BtlStageSetupUvAnim(int slot, u8 type, const char *material, float *params, int model)
{
    g_btlStageUvAnimTypes[slot] = type;
    g_btlStageUvAnimParams[slot] = params;
    switch (type) {
    case 1:
        GfxModelSetMaterialAnimCallback(g_btlArenaModels[model], material,
                                        BtlStageUvType1TexOffsetUCallback, params);
        break;
    case 2:
        GfxModelSetMaterialAnimCallback(g_btlArenaModels[model], material,
                                        BtlStageUvType2TexOffsetUCallback, params);
        break;
    case 3:
        GfxModelSetMaterialAnimCallback(g_btlArenaModels[model], material,
                                        BtlStageUvType3TexOffsetUCallback, params);
        break;
    case 4:
        GfxModelSetMaterialAnimCallback(g_btlArenaModels[model], material,
                                        BtlStageUvType4TexOffsetVCallback, params);
        break;
    default:
        break;
    }
}
