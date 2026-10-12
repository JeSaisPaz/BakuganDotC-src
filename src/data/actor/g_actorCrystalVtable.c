// bdc 0x08af1974 g_actorCrystalVtable
#include "bdc.h"

__typeof__(VtblEntry[49]) g_actorCrystalVtable = {
    {0}, { .fn = (void *)ActorCrystalDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)BtlBakuganSetMotionSpeed },
    { .fn = (void *)ActorCrystalUpdate }, { .fn = (void *)ActorCrystalDlWrite },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)ActorCrystalIsBakugan },
    { .fn = (void *)ActorCrystalIsCrystal }, { .fn = (void *)BtlBakuganIsUnitAlt },
    { .fn = (void *)BtlBakuganIsCpuUnit }, { .fn = (void *)BtlBakuganIsTargetPoint },
    { .fn = (void *)BtlBakuganIsPropTarget }, { .fn = (void *)BtlBakuganIsLandmarkAttrTarget },
    { .fn = (void *)ActorCrystalIsUntargetable }, { .fn = (void *)BtlBakuganIsMode4Unit },
    { .fn = (void *)BtlBakuganIsLandmarkTarget }, { .fn = (void *)ActorCrystalGetAttribute },
    { .fn = (void *)ActorCrystalSetHitTimer }, { .fn = (void *)BtlBakuganIsHpAtOrBelowThreshold },
    { .fn = (void *)ActorCrystalUpdateBoneAnchors }, { .fn = (void *)ActorCrystalOnHit },
    { .fn = (void *)BtlBakuganTryBlock }, { .fn = (void *)ActorCrystalModeIdle },
    { .fn = (void *)BtlBakuganState01Update }, { .fn = (void *)BtlBakuganState02Update },
    { .fn = (void *)BtlBakuganState03Update }, { .fn = (void *)BtlBakuganState04Update },
    { .fn = (void *)BtlBakuganState05Update }, { .fn = (void *)BtlBakuganState06Update },
    { .fn = (void *)ActorCrystalModeAttack }, { .fn = (void *)BtlBakuganState08Update },
    { .fn = (void *)BtlBakuganState09Update }, { .fn = (void *)BtlBakuganState10Update },
    { .fn = (void *)BtlBakuganState11Update }, { .fn = (void *)BtlBakuganState12Update },
    { .fn = (void *)BtlBakuganState13Update }, { .fn = (void *)BtlBakuganState14Update },
    { .fn = (void *)BtlBakuganState15Update }, { .fn = (void *)BtlBakuganState16Update },
    { .fn = (void *)BtlBakuganState17Update }, { .fn = (void *)BtlBakuganState18Update },
    { .fn = (void *)BtlBakuganState19Update }, { .fn = (void *)BtlBakuganState20Update },
    { .fn = (void *)BtlBakuganState21Update }, { .fn = (void *)BtlBakuganRunState },
};
