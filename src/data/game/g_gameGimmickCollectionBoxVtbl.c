// bdc 0x08af2f8c g_gameGimmickCollectionBoxVtbl
#include "bdc.h"

__typeof__(const VtblEntry[20]) g_gameGimmickCollectionBoxVtbl = {
    {0}, { .fn = (void *)GameGimmickCollectionBoxDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GameGimmickDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)GameGimmickCollectionBoxUpdate },
    { .fn = (void *)GameGimmickCollectionBoxDraw }, { .fn = (void *)GfxModelSetToon },
    { .fn = (void *)GameGimmickIsSwitch }, { .fn = (void *)GameGimmickIsBarrier },
    { .fn = (void *)GameGimmickIsTriggerZone }, { .fn = (void *)GameGimmickIsTouchSpot },
    { .fn = (void *)GameGimmickIsSolid }, { .fn = (void *)GameGimmickIsIrSensor },
    { .fn = (void *)GameGimmickDisable }, { .fn = (void *)GameGimmickIsHiddenOnRadar },
    { .fn = (void *)GameGimmickGetPos }, { .fn = (void *)GameGimmickGetCollisionShape },
};
