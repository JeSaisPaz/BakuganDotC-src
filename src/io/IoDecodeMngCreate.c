// bdc 0x089fe070 IoDecodeMngCreate
#include "bdc.h"

/* Creates the decode manager singleton (`g_ioDecodeMng`, 0x34 bytes, `IoDecodeMngCtor`), names
   its list `"CODecodeMng::m_pcListMng"` and registers it (`CoreNodeRegistryAdd`). Returns it.
   Called by `BootDecodeThread`. */

IoDecodeMng *IoDecodeMngCreate(void)

{
  bool fromLow;
  IoDecodeMng *self;
  IoDecodeMng *node;
  
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  self = MemAlloc(0x34,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  node = (IoDecodeMng *)0x0;
  if (self != (IoDecodeMng *)0x0) {
    IoDecodeMngCtor(self);
    node = self;
  }
  g_ioDecodeMng = node;
  (node->base).unk2c = g_ioDecodeMngListName;
  CoreNodeRegistryAdd((CoreNode *)node);
  return g_ioDecodeMng;
}

