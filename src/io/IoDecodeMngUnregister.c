// bdc 0x089fe110 IoDecodeMngUnregister
#include "bdc.h"

/* Removes the decode manager's list `"CODecodeMng::m_pcListMng"` from the node registry
   (`CoreNodeRegistryRemoveByName`). Called by `BootDecodeThread` on exit. */

void IoDecodeMngUnregister(void)

{
  CoreNodeRegistryRemoveByName(g_ioDecodeMngListName);

}

