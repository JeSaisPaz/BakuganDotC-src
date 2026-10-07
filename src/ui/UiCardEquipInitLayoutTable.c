// bdc 0x08969c80 UiCardEquipInitLayoutTable
#include "bdc.h"

/* Fills the sprite-group table `groups` of `UiCardEquip` ({first, count} byte
   pairs per group) for the small (fewer than 3 Bakugan, groups 0..20) or large layout (groups
   0..21); in the large layout also insets the UVs of the group-18 sprite and of every group-10
   sprite by half a texel (`GfxSpriteInsetUv`). */

void UiCardEquipInitLayoutTable(UiCardEquip *self)
{
  int i;

  if (self->bakuganCount < 3) {
    self->groups[0][0] = 0;
    self->groups[0][1] = 5;
    self->groups[1][0] = 5;
    self->groups[1][1] = 2;
    self->groups[2][0] = 7;
    self->groups[2][1] = 4;
    self->groups[3][0] = 11;
    self->groups[3][1] = 4;
    self->groups[4][0] = 15;
    self->groups[4][1] = 4;
    self->groups[5][0] = 19;
    self->groups[5][1] = 4;
    self->groups[6][0] = 23;
    self->groups[6][1] = 2;
    self->groups[7][0] = 25;
    self->groups[7][1] = 2;
    self->groups[8][0] = 27;
    self->groups[8][1] = 8;
    self->groups[9][0] = 35;
    self->groups[9][1] = 8;
    self->groups[10][0] = 43;
    self->groups[10][1] = 8;
    self->groups[11][0] = 51;
    self->groups[11][1] = 2;
    self->groups[12][0] = 53;
    self->groups[12][1] = 2;
    self->groups[13][0] = 55;
    self->groups[13][1] = 8;
    self->groups[14][0] = 63;
    self->groups[14][1] = 12;
    self->groups[15][0] = 75;
    self->groups[15][1] = 4;
    self->groups[16][0] = 79;
    self->groups[16][1] = 8;
    self->groups[17][0] = 87;
    self->groups[17][1] = 8;
    self->groups[18][0] = 95;
    self->groups[18][1] = 1;
    self->groups[19][0] = 102;
    self->groups[19][1] = 2;
    self->groups[20][0] = 96;
    self->groups[20][1] = 6;
  }
  else {
    self->groups[0][0] = 0;
    self->groups[0][1] = 9;
    self->groups[1][0] = 9;
    self->groups[1][1] = 4;
    self->groups[2][0] = 13;
    self->groups[2][1] = 6;
    self->groups[3][0] = 19;
    self->groups[3][1] = 6;
    self->groups[4][0] = 25;
    self->groups[4][1] = 6;
    self->groups[5][0] = 31;
    self->groups[5][1] = 6;
    self->groups[6][0] = 37;
    self->groups[6][1] = 4;
    self->groups[7][0] = 41;
    self->groups[7][1] = 2;
    self->groups[8][0] = 133;
    self->groups[8][1] = 16;
    self->groups[9][0] = 43;
    self->groups[9][1] = 16;
    self->groups[10][0] = 149;
    self->groups[10][1] = 16;
    self->groups[11][0] = 59;
    self->groups[11][1] = 4;
    self->groups[12][0] = 63;
    self->groups[12][1] = 4;
    self->groups[13][0] = 67;
    self->groups[13][1] = 16;
    self->groups[14][0] = 83;
    self->groups[14][1] = 24;
    self->groups[15][0] = 107;
    self->groups[15][1] = 8;
    self->groups[16][0] = 115;
    self->groups[16][1] = 16;
    self->groups[17][0] = 165;
    self->groups[17][1] = 16;
    self->groups[18][0] = 181;
    self->groups[18][1] = 1;
    self->groups[19][0] = 194;
    self->groups[19][1] = 4;
    self->groups[20][0] = 182;
    self->groups[20][1] = 12;
    self->groups[21][0] = 131;
    self->groups[21][1] = 2;
    GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[self->groups[18][0]]);
    for (i = self->groups[10][0]; i < self->groups[10][0] + (s8)self->groups[10][1]; i++) {
      GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[i]);
    }
  }
}
