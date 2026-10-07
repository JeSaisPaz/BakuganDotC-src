// bdc 0x0891ebbc UiHologramGalleryOpenViewPhase
#include "bdc.h"

/* Phase 3 of the hologram gallery: on step 0 sets `g_uiKeepSharedBg` and creates task 392
   (`UiHologramViewCtor`, `CoreTaskCreateDefault(0x188, pendingHint)`), on step 1 waits for it to
   close, then picks the phase-2 step from `pendingHint` (refreshing the cursor with
   `UiHologramGalleryUpdateCursor` for hints 2, 3 and 7) and returns to phase 2. Every frame it
   also steps the blink/scroll records and the slot, list and arrow animations. */

void UiHologramGalleryOpenViewPhase(UiHologramGallery *self)
{
    s32 step = self->base.phaseStep;

    if (step == 0) {
        g_uiKeepSharedBg = 1;
        CoreTaskCreateDefault(0x188, (void *)(uintptr_t)self->pendingHint);
        self->base.phaseStep = self->base.phaseStep + 1;
    } else if (step == 1) {
        if (CoreTaskExists(0x188) == 0) {
            self->base.phaseStep = 2;
        }
    } else {
        switch (self->pendingHint) {
        case 1:
            self->base.phaseStep = 0x20;
            break;
        case 2:
            UiHologramGalleryUpdateCursor(self);
            self->base.phaseStep = 0xd;
            break;
        case 3:
            UiHologramGalleryUpdateCursor(self);
            self->base.phaseStep = 0x1a;
            break;
        case 4:
            self->base.phaseStep = 0x1b;
            break;
        case 7:
            UiHologramGalleryUpdateCursor(self);
            self->base.phaseStep = 0x1a;
            break;
        default: /* 5, 6, 8, 9 and anything else */
            self->base.phaseStep = 1;
            break;
        }
        self->base.phase = 2;
    }
    UiCellBlinkUpdate(&self->cellBlink);
    UiScrollLoopUpdate(&self->scrollLoop);
    UiHologramGalleryPulseSlots(self);
    UiHologramGalleryPulseList(self);
    UiHologramGalleryAnimateScrollArrows(self);
}
