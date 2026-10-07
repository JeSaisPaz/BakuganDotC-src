// bdc 0x08af3314 g_gameGimmickVtbl
#include "bdc.h"

__typeof__(const VtblEntry[20]) g_gameGimmickVtbl = {
    {0}, { .fn = (void *)GameGimmickDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GameGimmickDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)GameGimmickUpdate }, { .fn = (void *)GameGimmickDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)GameGimmickIsSwitch },
    { .fn = (void *)GameGimmickIsBarrier }, { .fn = (void *)GameGimmickIsTriggerZone },
    { .fn = (void *)GameGimmickIsTouchSpot }, { .fn = (void *)GameGimmickIsSolid },
    { .fn = (void *)GameGimmickIsIrSensor }, { .fn = (void *)GameGimmickDisable },
    { .fn = (void *)GameGimmickIsHiddenOnRadar }, { .fn = (void *)GameGimmickGetPos },
    { .fn = (void *)GameGimmickGetCollisionShape },
};
