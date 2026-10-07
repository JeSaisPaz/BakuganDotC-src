// bdc 0x08af186c g_btlCameraVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_btlCameraVtbl = { {0}, { .fn = (void *)BtlCameraDtor }, { .fn = (void *)BtlCameraUpdate } };
