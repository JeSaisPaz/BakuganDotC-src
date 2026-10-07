// bdc 0x08af4b5c g_uiPasscodeVtbl
#include "bdc.h"

__typeof__(VtblEntry[2]) g_uiPasscodeVtbl = { {0}, { .fn = (void *)UiPasscodeDtor } };
