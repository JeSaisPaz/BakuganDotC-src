// bdc 0x08af2e2c g_gameGimmickCollidableVtbl
#include "bdc.h"

__typeof__(const VtblEntry[20]) g_gameGimmickCollidableVtbl = {
    {0}, { .fn = (void *)GameGimmickCollidableDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GameGimmickDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)GameGimmickCollidableUpdate }, { .fn = (void *)GameGimmickCollidableDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)GameGimmickIsSwitch },
    { .fn = (void *)GameGimmickIsBarrier }, { .fn = (void *)GameGimmickIsTriggerZone },
    { .fn = (void *)GameGimmickIsTouchSpot }, { .fn = (void *)GameGimmickIsSolid },
    { .fn = (void *)GameGimmickIsIrSensor }, { .fn = (void *)GameGimmickDisable },
    { .fn = (void *)GameGimmickIsHiddenOnRadar }, { .fn = (void *)GameGimmickCollidableGetPos },
    { .fn = (void *)GameGimmickCollidableGetCollisionShape },
};
