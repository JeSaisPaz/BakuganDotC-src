// bdc 0x089788f0 UiCollectionSphereGetCat2EntryId
#include "bdc.h"

/* Returns entry `index` of the id tables of category 2 of
   `UiCollectionSphere` (`0x08ac3558`, 15 bytes, or `0x08ac3567`, 12
   bytes). */

u8 UiCollectionSphereGetCat2EntryId(UiCollectionSphere *self, u8 special, u8 index)

{
  u8 result;
  u8 idsA [16];
  u8 idsB [12];
  
  memcpy(idsA,g_collectionSphereCat2IdsA,0xf);
  memcpy(idsB,g_collectionSphereCat2IdsB,0xc);
  if (special == '\0') {
    result = idsA[index];
  }
  else {
    result = idsB[index];
  }
  return result;
}

