// bdc 0x08af3734 g_gameGimmickSwitchVtbl
#include "bdc.h"

__typeof__(const VtblEntry[20]) g_gameGimmickSwitchVtbl = {
    {0}, { .fn = (void *)GameGimmickSwitchDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GameGimmickDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)GameGimmickSwitchUpdate }, { .fn = (void *)GameGimmickSwitchDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)GameGimmickSwitchIsSwitch },
    { .fn = (void *)GameGimmickIsBarrier }, { .fn = (void *)GameGimmickIsTriggerZone },
    { .fn = (void *)GameGimmickIsTouchSpot }, { .fn = (void *)GameGimmickIsSolid },
    { .fn = (void *)GameGimmickIsIrSensor }, { .fn = (void *)GameGimmickSwitchDisable },
    { .fn = (void *)GameGimmickIsHiddenOnRadar }, { .fn = (void *)GameGimmickSwitchGetPos },
    { .fn = (void *)GameGimmickSwitchGetCollisionShape },
};
