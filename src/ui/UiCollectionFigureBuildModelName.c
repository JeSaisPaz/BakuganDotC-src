// bdc 0x0898c1f0 UiCollectionFigureBuildModelName
#include "bdc.h"

/* Builds the figure GMO file name of entry `id` of `UiCollectionFigure`
   into `out` (64 bytes) from the 21-entry table `g_figureModelNames` (e.g.
   `"00_P_Dragonoid_N_U_figure.gmo"`). `self` and `category` are unused. */

void UiCollectionFigureBuildModelName(UiCollectionFigure *self, u8 category, u8 id, char *out)

{
  const char *names[21];
  char buf[64];
  u32 i;

  memcpy(names, g_figureModelNames, sizeof(names));
  memset(buf, 0, sizeof(buf));
  sprintf(buf, names[id]);
  for (i = 0; i < 0x40; i++) {
    out[i] = buf[i];
  }
}
