// bdc 0x089bdb50 IoLzsPackagePoll
#include "bdc.h"

/* Polls an `.lzs` package load started by `IoLzsPackageStartLoad`. Returns 0 while there is no
   load request (`+0x2c`) or `IoDataIsDone` reports it unfinished. Once done, if the package is
   not yet registered (`+0x2a == 0`), it fetches the loaded directory (`IoDataGetBuffer`) into
   `+0x24`, calls `IoLzsPackageRegister(self, dir, flag, 1)`, and adds the node to the
   loaded-package list `g_ioLzsPackages`: it becomes the head when the list is empty, otherwise
   `CoreNodeLink` mode 0 appends it at the end of the head's chain. Returns 1 when done (also on
   later polls of an already registered package). */

int IoLzsPackagePoll(IoLzsPackage *self, u8 flag)
{
  u16 *dir;

  if (self->request == NULL || !IoDataIsDone(self->request)) {
    return 0;
  }
  if (self->registered == 0) {
    dir = IoDataGetBuffer(self->request);
    self->dir = dir;
    IoLzsPackageRegister(self, dir, flag, 1);
    if (g_ioLzsPackages == NULL) {
      g_ioLzsPackages = self;
    } else {
      CoreNodeLink(&self->base, &g_ioLzsPackages->base, 0);
    }
  }
  return 1;
}
