// bdc 0x08af2c34 g_actorStageObjDebrisVtable
#include "bdc.h"

__typeof__(VtblEntry[10]) g_actorStageObjDebrisVtable = {
    {0}, { .fn = (void *)ActorStageObjDebrisDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorStageObjDebrisUpdate }, { .fn = (void *)GfxModelDlWriteState },
    { .fn = (void *)GfxModelSetToon },
};
