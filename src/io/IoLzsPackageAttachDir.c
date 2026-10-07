// bdc 0x089bd9c4 IoLzsPackageAttachDir
#include "bdc.h"

/* Attaches a loaded directory to an existing package node: clears the owns-data flag `+0x28`,
   stores `dir` at `+0x24`, optionally registers it (`IoLzsPackageRegister` with `fromLow = 1`),
   links the node into the loaded-package list `*0x08ac520c` when `parent` is 0, and clears the
   load-request words `+0x2c`/`+0x30`. */

void IoLzsPackageAttachDir(IoLzsPackage *self, u16 *dir, int parent, u8 doRegister, u8 createTextures)

{
  self->ownsDir = '\0';
  self->dir = dir;
  if (doRegister != '\0') {
    IoLzsPackageRegister(self,self->dir,createTextures,'\x01');
  }
  if (parent == 0) {
    if (g_ioLzsPackages == (IoLzsPackage *)0x0) {
      g_ioLzsPackages = self;
    } else {
      CoreNodeLink(&self->base,&g_ioLzsPackages->base,'\0');
    }
  }
  self->request = (void *)0x0;
  self->owner = (void *)0x0;
  return;
}

