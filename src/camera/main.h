#pragma once

rw::World*
CreateWorld(void);

void
LightsCreate(rw::World *world);

rw::Clump*
ClumpCreate(rw::World *world);

void
ClumpRotate(rw::Clump *clump, rw::Camera *camera, float xAngle, float yAngle);

void
ClumpTranslate(rw::Clump *clump, rw::Camera *camera, float xDelta, float yDelta);

void
ClumpSetPosition(rw::Clump *clump, rw::V3d *position);

bool
Initialize3D(void);

void
DestroyLight(rw::Light *light, rw::World *world);

void
Terminate3D(void);

void
DisplayOnScreenInfo(void);

void
ResetCameraAndClump(void);

void
Gui(void);

void
MainCameraRender(rw::Camera *camera);

void
SubCameraRender(rw::Camera *camera);

void
Render(void);

void
Idle(float timeDelta);

extern bool RotateClump;
extern bool TranslateClump;
extern bool RotateCamera;
extern bool TranslateCamera;
extern bool ViewXWindow;
extern bool ViewYWindow;
extern bool ViewXOffset;
extern bool ViewYOffset;

void
MouseMove(int mouseposx, int mouseposy);
