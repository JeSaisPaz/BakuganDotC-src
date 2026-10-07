// bdc 0x08af5954 g_uiWindowFrameVtable
#include "bdc.h"

__typeof__(const VtblEntry[13]) g_uiWindowFrameVtable = {
    {0}, { .fn = (void *)UiWindowFrameDtor }, { .fn = (void *)UiWindowFrameUpdate },
    { .fn = (void *)UiWindowFrameDraw }, { .fn = (void *)UiWindowFrameOpen },
    { .fn = (void *)UiWindowFrameClose }, { .fn = (void *)UiWindowFrameApplyColors },
    { .fn = (void *)UiWindowFrameLayout }, { .fn = (void *)UiWindowFrameSetStyle },
    { .fn = (void *)UiWindowFrameSetAlpha }, { .fn = (void *)UiWindowFrameAddAlpha },
    { .fn = (void *)UiWindowFrameSetHighlight }, { .fn = (void *)UiWindowFrameClearHighlight },
};
