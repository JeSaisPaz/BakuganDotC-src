// bdc 0x08804790 UiNameEntryCtor
#include "bdc.h"

/* Constructor of the player name entry screen, task id 3000 (0xbb8) (base `UiScreenCtor`, vtable
   `g_uiNameEntryVtbl`). Object size 0xf8. Clears its fields, creates a camera object
   (`GfxSetActiveCamera(NULL)`) at `camera`, fills the 12 `name` slots with -1, sets frame mode 1,
   makes the stick emulate the d-pad and sets the camera up (`GfxCameraInit`, target (0, 140, 0),
   eye (0, 140, 280), screen offset (-180, -90), near 90, far 350, fov 35, then
   `GfxCameraUpdate` with all flags). The screen loads `"data/2d/<lang>/name.lzs"` and
   `"data/name_common.lzs"`, shows the player avatar (`"12_Edit_man.gmo"` with `"editman_mot.gmo"`
   motions on the `"menu_daiza.gmo"` pedestal) and lets the player type a name; the result goes to
   the save profile (default `"No Name"`). Returns `self`. */

UiNameEntry *UiNameEntryCtor(UiNameEntry *self)
{
  GfxCamera *cam;
  int i;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiNameEntryVtbl;
  self->langPack = NULL;
  self->commonPack = NULL;
  self->done = 0;
  self->nameLength = 0;
  self->keyCol = 0;
  self->keyRow = 0;
  self->page = 0;
  self->confirmed = 0;
  self->symbolPage = 0;
  self->repeatTimer = 0;
  self->keyText = NULL;
  self->nameText = NULL;
  self->avatar = NULL;
  self->camera = GfxSetActiveCamera(NULL);
  self->motion = -1;
  self->baseModel = NULL;
  self->baseNode = NULL;
  self->baseMatrix = NULL;
  self->introSoundTimer = 0;
  self->confirmTimer = 0;
  self->cursorAngle = 0;
  self->keyPressActive = 0;
  self->keyPressFrame = 0;
  self->flashProgress = 0.0f;
  for (i = 0; i < 12; i++) {
    self->name[i] = -1;
  }
  UiScreenSetFrameMode((CoreTask *)self, 1);
  self->base.pad->stickEmulatesDpad = 1;
  GfxCameraInit((GfxCamera *)self->camera);
  cam = (GfxCamera *)self->camera;
  cam->target[0] = 0.0f;
  cam->target[1] = 140.0f;
  cam->target[2] = 0.0f;
  cam->target[3] = 0.0f;
  cam = (GfxCamera *)self->camera;
  cam->eye[0] = 0.0f;
  cam->eye[1] = 140.0f;
  cam->eye[2] = 280.0f;
  cam->eye[3] = 0.0f;
  GfxCameraSetScreenOffset(-180.0f, -90.0f, (GfxCamera *)self->camera);
  ((GfxCamera *)self->camera)->nearZ = 90.0f;
  ((GfxCamera *)self->camera)->farZ = 350.0f;
  ((GfxCamera *)self->camera)->fov = 35.0f;
  GfxCameraUpdate((GfxCamera *)self->camera, 0xffffffff);
  return self;
}
