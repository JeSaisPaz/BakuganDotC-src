// bdc 0x089cfa5c NetCharaFindOrCreate
#include "bdc.h"

/* Returns the net character for `mac` (`NetCharaFindByMac`) or, when none exists, allocates a
   0x15c-byte one from the low heap and constructs it with `NetCharaCtor`. Callers pass a source
   path and line (`"…/Net/CONetChara.cpp"`, `"…/Net/CONetPDP.cpp"`) that the release build
   ignores, so Ghidra shows only `mac`. */

void *NetCharaFindOrCreate(u8 *mac)

{
  bool fromLow;
  NetChara *found;
  NetChara *self;
  
  found = NetCharaFindByMac(mac);
  if (found == (NetChara *)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    self = MemAlloc(0x15c,(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    found = (NetChara *)0x0;
    if (self != (NetChara *)0x0) {
      NetCharaCtor(self,mac);
      found = self;
    }
  }
  return found;
}

