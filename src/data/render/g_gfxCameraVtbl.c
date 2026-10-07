// bdc 0x08af54d4 g_gfxCameraVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_gfxCameraVtbl = { {0}, { .fn = (void *)GfxCameraDtor }, { .fn = (void *)GfxCameraOnUpdate } };
