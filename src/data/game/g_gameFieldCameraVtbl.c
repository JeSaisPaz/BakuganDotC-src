// bdc 0x08af2ce4 g_gameFieldCameraVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_gameFieldCameraVtbl = { {0}, { .fn = (void *)GameFieldCameraDtor }, { .fn = (void *)GameFieldCameraUpdate } };
