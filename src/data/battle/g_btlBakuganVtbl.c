// bdc 0x08af1fa4 g_btlBakuganVtbl
#include "bdc.h"

__typeof__(VtblEntry[49]) g_btlBakuganVtbl = {
    {0}, { .fn = (void *)BtlBakuganDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)BtlBakuganSetMotionSpeed },
    { .fn = (void *)BtlBakuganUpdate }, { .fn = (void *)BtlBakuganDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)BtlBakuganIsBakugan },
    { .fn = (void *)BtlBakuganIsCrystal }, { .fn = (void *)BtlBakuganIsUnitAlt },
    { .fn = (void *)BtlBakuganIsCpuUnit }, { .fn = (void *)BtlBakuganIsTargetPoint },
    { .fn = (void *)BtlBakuganIsPropTarget }, { .fn = (void *)BtlBakuganIsLandmarkAttrTarget },
    { .fn = (void *)BtlBakuganIsUntargetable }, { .fn = (void *)BtlBakuganIsMode4Unit },
    { .fn = (void *)BtlBakuganIsLandmarkTarget }, { .fn = (void *)BtlBakuganGetAttribute },
    { .fn = (void *)BtlBakuganSetHpThreshold }, { .fn = (void *)BtlBakuganIsHpAtOrBelowThreshold },
    { .fn = (void *)BtlBakuganUpdateBoneAnchors }, { .fn = (void *)BtlBakuganOnHit },
    { .fn = (void *)BtlBakuganTryBlock }, { .fn = (void *)BtlBakuganState00Update },
    { .fn = (void *)BtlBakuganState01Update }, { .fn = (void *)BtlBakuganState02Update },
    { .fn = (void *)BtlBakuganState03Update }, { .fn = (void *)BtlBakuganState04Update },
    { .fn = (void *)BtlBakuganState05Update }, { .fn = (void *)BtlBakuganState06Update },
    { .fn = (void *)BtlBakuganState07Update }, { .fn = (void *)BtlBakuganState08Update },
    { .fn = (void *)BtlBakuganState09Update }, { .fn = (void *)BtlBakuganState10Update },
    { .fn = (void *)BtlBakuganState11Update }, { .fn = (void *)BtlBakuganState12Update },
    { .fn = (void *)BtlBakuganState13Update }, { .fn = (void *)BtlBakuganState14Update },
    { .fn = (void *)BtlBakuganState15Update }, { .fn = (void *)BtlBakuganState16Update },
    { .fn = (void *)BtlBakuganState17Update }, { .fn = (void *)BtlBakuganState18Update },
    { .fn = (void *)BtlBakuganState19Update }, { .fn = (void *)BtlBakuganState20Update },
    { .fn = (void *)BtlBakuganState21Update }, { .fn = (void *)BtlBakuganRunState },
};
