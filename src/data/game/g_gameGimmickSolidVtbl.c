// bdc 0x08af35d4 g_gameGimmickSolidVtbl
#include "bdc.h"

__typeof__(const VtblEntry[20]) g_gameGimmickSolidVtbl = {
    {0}, { .fn = (void *)GameGimmickSolidDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GameGimmickDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)GameGimmickSolidUpdate }, { .fn = (void *)GameGimmickSolidDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)GameGimmickIsSwitch },
    { .fn = (void *)GameGimmickIsBarrier }, { .fn = (void *)GameGimmickIsTriggerZone },
    { .fn = (void *)GameGimmickIsTouchSpot }, { .fn = (void *)GameGimmickSolidIsSolid },
    { .fn = (void *)GameGimmickIsIrSensor }, { .fn = (void *)GameGimmickSolidDisable },
    { .fn = (void *)GameGimmickIsHiddenOnRadar }, { .fn = (void *)GameGimmickGetPos },
    { .fn = (void *)GameGimmickGetCollisionShape },
};
