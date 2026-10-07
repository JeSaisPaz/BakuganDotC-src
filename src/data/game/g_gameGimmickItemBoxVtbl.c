// bdc 0x08af30f4 g_gameGimmickItemBoxVtbl
#include "bdc.h"

__typeof__(const VtblEntry[21]) g_gameGimmickItemBoxVtbl = {
    {0}, { .fn = (void *)GameGimmickItemBoxDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GameGimmickDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)GameGimmickItemBoxUpdate }, { .fn = (void *)GameGimmickItemBoxDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)GameGimmickIsSwitch },
    { .fn = (void *)GameGimmickIsBarrier }, { .fn = (void *)GameGimmickIsTriggerZone },
    { .fn = (void *)GameGimmickIsTouchSpot }, { .fn = (void *)GameGimmickIsSolid },
    { .fn = (void *)GameGimmickIsIrSensor }, { .fn = (void *)GameGimmickItemBoxActivate },
    { .fn = (void *)GameGimmickIsHiddenOnRadar }, { .fn = (void *)GameGimmickGetPos },
    { .fn = (void *)GameGimmickGetCollisionShape }, { .fn = (void *)GameGimmickItemBoxRunState },
};
