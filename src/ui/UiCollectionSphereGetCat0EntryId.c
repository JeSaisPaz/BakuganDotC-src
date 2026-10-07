// bdc 0x08978820 UiCollectionSphereGetCat0EntryId
#include "bdc.h"

/* Returns entry `index` of the sphere id table for category 0 of
   `UiCollectionSphere`: from the 33-byte table `0x08ac34f8` (`special` =
   0) or the 19-byte table `0x08ac3519`. */

u8 UiCollectionSphereGetCat0EntryId(UiCollectionSphere *self, u8 special, u8 index)

{
  u8 result;
  u8 ids [36];
  u8 special_ids [20];
  
  memcpy(ids,g_uiCollectionSphereCat0IdsNormal,0x21);
  memcpy(special_ids,g_uiCollectionSphereCat0IdsSpecial,0x13);
  if (special == '\0') {
    result = ids[index];
  }
  else {
    result = special_ids[index];
  }
  return result;
}

