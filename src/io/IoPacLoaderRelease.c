// bdc 0x089fdbd0 IoPacLoaderRelease
#include "bdc.h"

/* Releases a loaded package (state 2) of the `.pac` package loader (`g_ioPacLoader`, 8 bytes:
   `+0` state, `+4` request/pack): drops all requests owned by the pack (`IoDataMngReleaseOwner`)
   and moves to state 3. Returns whether it was loaded. */

bool IoPacLoaderRelease(IoPacLoader *loader)
{
  int state = loader->state;

  if (state == 2) {
    IoDataMngReleaseOwner(IoGetDataMng(), loader->entry);
    loader->state = 3;
  }
  return state == 2;
}
