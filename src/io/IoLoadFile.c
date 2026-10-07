// bdc 0x089be374 IoLoadFile
#include "bdc.h"

/* Reads a whole file into memory and returns the buffer. If `rawPath` is 0 the name is first
   prefixed with the current storage root via `IoMakePath` (into a 256-byte stack buffer); the
   file is opened read-only, its size found by seeking to the end (stored in `*outSize` when
   non-NULL), and the contents are read into `buf`, or into a new heap block allocated from the low
   end of the heap when `buf` is NULL. The file is closed before returning. An empty file or a
   failed allocation closes the descriptor early but still falls through to the seek/read/close
   on the closed descriptor (as the original does), returning `buf` (NULL on allocation failure). */

void *IoLoadFile(const char *path, void *buf, u32 *outSize, char rawPath)
{
  SceOff zero = g_zeroFileOffsetA;
  bool fromLow;
  SceUID fd;
  SceSize size;
  char fullPath[256];

  if (rawPath == '\0') {
    fullPath[0] = '\0';
    IoMakePath(path, fullPath);
    path = fullPath;
  }
  fd = sceIoOpen(path, 1, 0x1ff);
  size = (SceSize)sceIoLseek(fd, zero, 2);
  if (size == 0) {
    sceIoClose(fd);
  }
  sceIoLseek(fd, zero, 0);
  if (outSize != NULL) {
    *outSize = size;
  }
  if (buf == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    buf = MemAlloc(size, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (buf == NULL) {
      sceIoClose(fd);
    }
  }
  sceIoRead(fd, buf, size);
  sceIoClose(fd);
  return buf;
}
