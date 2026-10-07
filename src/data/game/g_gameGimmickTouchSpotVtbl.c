// bdc 0x08af3684 g_gameGimmickTouchSpotVtbl
#include "bdc.h"

__typeof__(const VtblEntry[20]) g_gameGimmickTouchSpotVtbl = {
    {0}, { .fn = (void *)GameGimmickTouchSpotDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GameGimmickDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)GameGimmickTouchSpotUpdate }, { .fn = (void *)GameGimmickTouchSpotDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)GameGimmickIsSwitch },
    { .fn = (void *)GameGimmickIsBarrier }, { .fn = (void *)GameGimmickIsTriggerZone },
    { .fn = (void *)GameGimmickTouchSpotIsTouchSpot }, { .fn = (void *)GameGimmickIsSolid },
    { .fn = (void *)GameGimmickIsIrSensor }, { .fn = (void *)GameGimmickTouchSpotDisable },
    { .fn = (void *)GameGimmickIsHiddenOnRadar }, { .fn = (void *)GameGimmickGetPos },
    { .fn = (void *)GameGimmickTouchSpotGetCollisionShape },
};
