// bdc 0x089fd9dc IoPacLoaderLoad
#include "bdc.h"

/* Starts loading package file `path` with the `.pac` package loader (`g_ioPacLoader`, 8 bytes:
   `+0` state, `+4` request/pack): when idle and the data manager exists, requests the file
   (`IoDataMngRequest`, low heap, new request, path copied), sets its flags to 2 and moves to
   state 1. Returns 1 when started. */

int IoPacLoaderLoad(IoPacLoader *loader, char *path)
{
  IoData *req;
  int result = 0;

  if (loader->state == 0 && IoDataMngExists()) {
    req = IoDataMngRequest(IoGetDataMng(), loader, path, 1, true, true);
    loader->entry = req;
    if (req != (IoData *)0x0) {
      IoDataSetFlags(req, 2);
      result = 1;
      loader->state = 1;
    }
  }
  return result;
}
