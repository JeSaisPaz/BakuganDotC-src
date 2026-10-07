// bdc 0x08af1c94 g_btlUnitAltVtbl
#include "bdc.h"

__typeof__(VtblEntry[49]) g_btlUnitAltVtbl = {
    {0}, { .fn = (void *)BtlUnitAltDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)BtlBakuganSetMotionSpeed },
    { .fn = (void *)BtlUnitAltUpdate }, { .fn = (void *)BtlUnitAltDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)BtlUnitAltIsBakugan },
    { .fn = (void *)BtlBakuganIsCrystal }, { .fn = (void *)BtlUnitAltIsUnitAlt },
    { .fn = (void *)BtlUnitAltIsCpuUnit }, { .fn = (void *)BtlBakuganIsTargetPoint },
    { .fn = (void *)BtlBakuganIsPropTarget }, { .fn = (void *)BtlBakuganIsLandmarkAttrTarget },
    { .fn = (void *)BtlBakuganIsUntargetable }, { .fn = (void *)BtlBakuganIsMode4Unit },
    { .fn = (void *)BtlBakuganIsLandmarkTarget }, { .fn = (void *)BtlBakuganGetAttribute },
    { .fn = (void *)BtlBakuganSetHpThreshold }, { .fn = (void *)BtlBakuganIsHpAtOrBelowThreshold },
    { .fn = (void *)BtlUnitAltUpdateBoneAnchors }, { .fn = (void *)BtlBakuganOnHit },
    { .fn = (void *)BtlUnitAltTryBlock }, { .fn = (void *)BtlUnitAltState00Update },
    { .fn = (void *)BtlUnitAltState01Update }, { .fn = (void *)BtlUnitAltState02Update },
    { .fn = (void *)BtlUnitAltState03Update }, { .fn = (void *)BtlUnitAltState04Update },
    { .fn = (void *)BtlUnitAltState05Update }, { .fn = (void *)BtlUnitAltState06Update },
    { .fn = (void *)BtlUnitAltState07Update }, { .fn = (void *)BtlUnitAltState08Update },
    { .fn = (void *)BtlUnitAltState09Update }, { .fn = (void *)BtlUnitAltState10Update },
    { .fn = (void *)BtlUnitAltState11Update }, { .fn = (void *)BtlUnitAltState12Update },
    { .fn = (void *)BtlUnitAltState13Update }, { .fn = (void *)BtlUnitAltState14Update },
    { .fn = (void *)BtlUnitAltState15Update }, { .fn = (void *)BtlUnitAltState16Update },
    { .fn = (void *)BtlUnitAltState17Update }, { .fn = (void *)BtlUnitAltState18Update },
    { .fn = (void *)BtlUnitAltState19Update }, { .fn = (void *)BtlUnitAltState20Update },
    { .fn = (void *)BtlUnitAltState21Update }, { .fn = (void *)BtlUnitAltRunState },
};
