// bdc 0x08af4d44 g_uiBattleRecordVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_uiBattleRecordVtbl = { {0}, { .fn = (void *)UiBattleRecordDtor }, { .fn = (void *)UiBattleRecordUpdate } };
