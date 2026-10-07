// bdc 0x089bd89c IoLzsPackageInitWithDir
#include "bdc.h"

/* Constructs a package node around an already-loaded directory: `CoreNodeCtorMode``(pkg, parent,
   0)`, vtable `0x08af5214` (the `IoLzsPackageCtor` class), clears the flag bytes and texture
   array, stores `dir` at `+0x24`, optionally registers it (`IoLzsPackageRegister`), and when
   `parent` is NULL links it into the loaded-package list `*0x08ac520c` (becoming the head if the
   list was empty). Returns `pkg`. */

IoLzsPackage *IoLzsPackageInitWithDir(IoLzsPackage *self, u16 *dir, CoreNode *parent, u8 doRegister, u8 createTextures, u8 fromLow)

{
  CoreNodeCtorMode(&self->base,parent,'\0');
  (self->base).vtable = g_ioLzsPackageVtbl;
  self->flag29 = '\0';
  self->registered = '\0';
  self->ownsDir = '\0';
  self->textures = (void *)0x0;
  self->textureCount = 0;
  self->dir = dir;
  if (doRegister != '\0') {
    IoLzsPackageRegister(self,self->dir,createTextures,fromLow);
  }
  if (parent == (CoreNode *)0x0) {
    if (g_ioLzsPackages == (IoLzsPackage *)0x0) {
      g_ioLzsPackages = self;
    } else {
      CoreNodeLink(&self->base,&g_ioLzsPackages->base,'\0');
    }
  }
  self->request = (void *)0x0;
  self->owner = (void *)0x0;
  return self;
}

