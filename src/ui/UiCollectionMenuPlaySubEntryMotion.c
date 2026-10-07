// bdc 0x08976324 UiCollectionMenuPlaySubEntryMotion
#include "bdc.h"

/* On the sub-page of main entry 0 of `UiCollectionMenu`, plays the item-box
   motion that turns the box to the newly selected sub-entry, depending on `moveDir`:
   up (1): sub 0→7, 1→6, 2→5; down (2): sub 0→4, 1→2, 2→3. Any other case plays nothing.
   (The asm reads the sub selection as `(&selMain)[page]`, i.e. `selSub` since `page == 1`.) */

void UiCollectionMenuPlaySubEntryMotion(UiCollectionMenu *self)
{
    s8 sub;

    if (self->page != 1 || self->selMain != 0) {
        return;
    }
    if (self->moveDir == 1) {
        sub = self->selSub;
        if (sub == 0) {
            UiCollectionMenuPlayItemBoxMotion(self, 7);
        } else if (sub == 1) {
            UiCollectionMenuPlayItemBoxMotion(self, 6);
        } else if (sub == 2) {
            UiCollectionMenuPlayItemBoxMotion(self, 5);
        }
    } else if (self->moveDir == 2) {
        sub = self->selSub;
        if (sub == 0) {
            UiCollectionMenuPlayItemBoxMotion(self, 4);
        } else if (sub == 1) {
            UiCollectionMenuPlayItemBoxMotion(self, 2);
        } else if (sub == 2) {
            UiCollectionMenuPlayItemBoxMotion(self, 3);
        }
    }
}
