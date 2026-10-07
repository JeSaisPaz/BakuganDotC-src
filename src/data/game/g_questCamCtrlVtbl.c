// bdc 0x08af453c g_questCamCtrlVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_questCamCtrlVtbl = { {0}, { .fn = (void *)GameQuestCamCtrlDtor }, { .fn = (void *)GameQuestCamCtrlUpdate } };
