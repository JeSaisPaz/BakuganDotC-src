// bdc 0x08a9fb98 g_pspFreeHeapHook
#include "bdc.h"

__typeof__(void *) g_pspFreeHeapHook = (void *)__psp_free_heap;
