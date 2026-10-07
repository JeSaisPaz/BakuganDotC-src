// bdc 0x08978888 UiCollectionSphereGetCat1EntryId
#include "bdc.h"

/* Returns entry `index` of the id tables of category 1 of
   `UiCollectionSphere` (`0x08ac352c`, 33 bytes, or `0x08ac354d`, 11
   bytes). */

u8 UiCollectionSphereGetCat1EntryId(UiCollectionSphere *self, u8 special, u8 index)

{
  u8 result;
  u8 ids [36];
  u8 special_ids [12];
  
  memcpy(ids,g_uiCollectionSphereCat1IdsNormal,0x21);
  memcpy(special_ids,g_uiCollectionSphereCat1IdsSpecial,0xb);
  if (special == '\0') {
    result = ids[index];
  }
  else {
    result = special_ids[index];
  }
  return result;
}

