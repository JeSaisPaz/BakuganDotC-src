// bdc 0x08af1244 g_gmoImageVramAlloc
#include "bdc.h"

__typeof__(void *) g_gmoImageVramAlloc = (void *)malloc;
