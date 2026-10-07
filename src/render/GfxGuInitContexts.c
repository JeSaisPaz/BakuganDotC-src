// bdc 0x08a1f8f4 GfxGuInitContexts
#include "bdc.h"

/* Resets the libgu library state: display/draw buffer defaults (480x272, psm 1), the active context
   id (`g_guDeferredMode` = -1, no context started), the current context pointer, enable mask and
   flags, then the five `g_guContexts` (state defaults 4, 100, psm 1, 480x272, depth range 0..1,
   `ctxD4 = 0xffff`, `parentContext = -1` = free), and finally the signal/finish callbacks. */

void GfxGuInitContexts(void)
{
    int i;
    GuContext *ctx;

    g_guDispWidth = 480;
    g_guDeferredMode = -1;
    g_guDefaultsSent = 0;
    g_guDispBufWidth = 480;
    g_guDrawBufOffset = 0;
    g_guDispBufOffset = 0;
    g_guDepthBufPtr = 0;
    g_guDepthBufWidth = 0;
    g_guDisplayOn = 0;
    g_guCallMode = 0;
    g_guCurrentContext = 0;
    g_guEnableMask = 0;
    g_guPixelFormat = 1;
    g_guDispHeight = 272;

    ctx = g_guContexts;
    for (i = 4; i >= 0; i--) {
        ctx->ctx00 = 4;
        ctx->isSubList = 0;
        ctx->ctx94 = 100;
        ctx->ctx98 = 0;
        ctx->psm = 1;
        ctx->dispWidth = 480;
        ctx->dispHeight = 272;
        ctx->scissorEnable = 0;
        ctx->scissorX0 = 0;
        ctx->scissorY0 = 0;
        ctx->scissorX1 = 0;
        ctx->scissorY1 = 0;
        ctx->depthNear = 0;
        ctx->depthFar = 1;
        ctx->depthBias = 0;
        ctx->ctxBC[0] = 0;
        ctx->ctxBC[1] = 0;
        ctx->ctxBC[2] = 0;
        ctx->ctxBC[3] = 0;
        ctx->ctxBC[4] = 0;
        ctx->ctxD0 = 0;
        ctx->ctxD4 = 0xffff;
        ctx->fragment2x = 0;
        ctx->texComponent = 0;
        ctx->texFunction = 0;
        ctx->ctxF0 = 0;
        ctx->ctxF4 = 0;
        ctx->parentContext = -1;
        ctx++;
    }

    g_guSignalCallback = 0;
    g_guFinishCallback = 0;
}
