// bdc 0x08af234c g_gameGimmickCorePointVtbl
#include "bdc.h"

__typeof__(const VtblEntry[20]) g_gameGimmickCorePointVtbl = {
    {0}, { .fn = (void *)GameGimmickCorePointDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GameGimmickDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)GameGimmickCorePointUpdate }, { .fn = (void *)GameGimmickCorePointDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)GameGimmickIsSwitch },
    { .fn = (void *)GameGimmickIsBarrier }, { .fn = (void *)GameGimmickIsTriggerZone },
    { .fn = (void *)GameGimmickIsTouchSpot }, { .fn = (void *)GameGimmickIsSolid },
    { .fn = (void *)GameGimmickIsIrSensor }, { .fn = (void *)GameGimmickDisable },
    { .fn = (void *)GameGimmickCorePointIsHiddenOnRadar }, { .fn = (void *)GameGimmickGetPos },
    { .fn = (void *)GameGimmickCorePointGetCollisionShape },
};
