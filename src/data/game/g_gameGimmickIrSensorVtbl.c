// bdc 0x08af303c g_gameGimmickIrSensorVtbl
#include "bdc.h"

__typeof__(const VtblEntry[21]) g_gameGimmickIrSensorVtbl = {
    {0}, { .fn = (void *)GameGimmickIrSensorDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GameGimmickDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)GameGimmickIrSensorUpdate }, { .fn = (void *)GameGimmickIrSensorDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)GameGimmickIsSwitch },
    { .fn = (void *)GameGimmickIsBarrier }, { .fn = (void *)GameGimmickIsTriggerZone },
    { .fn = (void *)GameGimmickIsTouchSpot }, { .fn = (void *)GameGimmickIsSolid },
    { .fn = (void *)GameGimmickIrSensorIsIrSensor }, { .fn = (void *)GameGimmickDisable },
    { .fn = (void *)GameGimmickIsHiddenOnRadar }, { .fn = (void *)GameGimmickGetPos },
    { .fn = (void *)GameGimmickGetCollisionShape }, { .fn = (void *)GameGimmickIrSensorRunState },
};
