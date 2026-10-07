// bdc 0x08804484 GfxBootTextureReplace
#include "bdc.h"

/* Overwrites the decoded image of boot texture `name` (an entry of `g_bootTextureTable`, 14 `{lzs
   data, name}` pairs) inside the boot texture buffer `g_bootTextureData` with replacement bytes:
   the destination offset is the sum of the decoded sizes (`CoreLzssGetSize`) of the entries before
   it. When `data` is NULL it uses `"<name>.rep"` from the package chain `g_ioLzsPackages`
   (`CorePackChainFind` / `CorePackChainFindSize`). Returns 1 when something was copied, 0 when
   the buffer is missing, `name` is NULL, the offset is 0 (first entry) or no data was found. */

s32 GfxBootTextureReplace(const char *name, const void *data, u32 size)
{
  IoLzsPackage *packChain;
  void **entry;
  u32 index;
  s32 offset;
  u8 *dst;
  s32 copied;
  char repName[128];

  copied = 0;
  if (g_bootTextureData != NULL && name != NULL) {
    offset = 0;
    index = 0;
    entry = g_bootTextureTable;
    do {
      if (strcmp(entry[1], name) == 0) break;
      index++;
      offset += CoreLzssGetSize(entry[0]);
      entry += 2;
    } while (index < 14);
    if ((s32)index >= 0 && offset > 0) {
      dst = g_bootTextureData + offset;
      if (data == NULL) {
        packChain = g_ioLzsPackages;
        if (packChain != NULL) {
          sprintf(repName, "%s.rep", name);
          data = CorePackChainFind(packChain, repName);
          size = CorePackChainFindSize(packChain, repName);
        }
      }
      if (data != NULL) {
        memcpy(dst, data, size);
        copied = 1;
      }
    }
  }
  return copied;
}
