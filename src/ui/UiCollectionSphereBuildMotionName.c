// bdc 0x0897d960 UiCollectionSphereBuildMotionName
#include "bdc.h"

/* Builds the pop-out motion name of entry `id` of `UiCollectionSphere`
   into `out` (64 bytes): from `g_spherePopoutMotionNames` (e.g. `"00_dor_dir_popout"`) for
   categories 0/1 and category-2 model pages, from `g_sphereSpecialMotionNames` (e.g.
   `"00_M_Dragonoid_N_P_fencer2_motion"`) on a category-2 special-item page (page kind 0 of
   the current `page`). The name is passed to `sprintf` as the format string. */

void UiCollectionSphereBuildMotionName(UiCollectionSphere *self, u8 category, u8 id, char *out)

{
  char buf[64];
  const char *names[33];
  const char *specialNames[13];
  u32 i;

  memcpy(names, g_spherePopoutMotionNames, sizeof(names));
  memcpy(specialNames, g_sphereSpecialMotionNames, sizeof(specialNames));
  memset(buf, 0, sizeof(buf));
  if (category < 2) {
    sprintf(buf, names[id]);
  }
  else if (UiCollectionSphereGetPageKind(self, (u8)self->page) == 0) {
    sprintf(buf, specialNames[id]);
  }
  else {
    sprintf(buf, names[id]);
  }
  for (i = 0; i < 0x40; i++) {
    out[i] = buf[i];
  }
}
