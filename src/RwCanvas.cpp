#include <skeleton/skeleton.h>
#include "RwStarterApp.h"
#include "viewer.h"
#include "RwGLContext.h"
#include "RwCanvas.h"

#define TEXSIZE 256

rw::EngineOpenParams engineOpenParams;

sk::EventStatus AppEventHandler(sk::Event e, void* param)
{
	return sk::EVENTNOTPROCESSED;
}

wxBEGIN_EVENT_TABLE(RwCanvas, wxRwCanvas)
    EVT_MOTION(RwCanvas::OnMouseMoved)
wxEND_EVENT_TABLE()

RwCanvas::RwCanvas(wxWindow* parent)
: wxRwCanvas(parent)
{
    m_lastMousePos = wxGetMousePosition();
}

RwCanvas::~RwCanvas()
{
    Terminate3D();
}

wxRwGLContext* RwCanvas::InitContext()
{
    RwGLContext* context = wxGetApp().GetContext(this);
    return context;
}

void RwCanvas::InitRwResources()
{
    Initialize3D();
}

void RwCanvas::Render()
{
    const wxSize size = GetClientSize() * GetContentScaleFactor();

    rw::Rect rect;
    rect.x = rect.y = 0;
    rect.w = size.GetWidth();
    rect.h = size.GetHeight();

    CameraSizeUpdate(&rect, 0.5f, ((float)rect.h) / rect.w);

    Idle(0.01f);

    SwapBuffers();
}

void RwCanvas::OnMouseMoved(wxMouseEvent& event)
{
    wxPoint pos = wxGetMousePosition();
    wxPoint delta = pos - m_lastMousePos;
    m_lastMousePos = pos;

    RotateClump = !event.ControlDown() && !event.AltDown() && !event.ShiftDown() && event.LeftIsDown();
    TranslateClump = !event.ControlDown() && !event.AltDown() && !event.ShiftDown() && event.RightIsDown();
    RotateCamera = event.ControlDown() && event.LeftIsDown();
    TranslateCamera = event.ControlDown() && event.RightIsDown();
    ViewXWindow = event.ShiftDown() && event.LeftIsDown();
    ViewYWindow = event.ShiftDown() && event.RightIsDown();
    ViewXOffset = event.AltDown() && event.LeftIsDown();
    ViewYOffset = event.AltDown() && event.RightIsDown();

    MouseMove(delta.x, delta.y);
}

rw::World* RwCanvas::CreateWorld(void)
{
	rw::BBox bb;

	bb.inf.x = bb.inf.y = bb.inf.z = -100.0f;
	bb.sup.x = bb.sup.y = bb.sup.z = 100.0f;

	return rw::World::create(&bb);
}

void RwCanvas::LightsCreate(rw::World* world)
{
	rw::Light* light = rw::Light::create(rw::Light::AMBIENT);
	assert(light);
	World->addLight(light);

	light = rw::Light::create(rw::Light::DIRECTIONAL);
	assert(light);
	rw::Frame* frame = rw::Frame::create();
	assert(frame);
	frame->rotate(&Xaxis, 30.0f, rw::COMBINEREPLACE);
	frame->rotate(&Yaxis, 30.0f, rw::COMBINEPOSTCONCAT);
	light->setFrame(frame);
	World->addLight(light);
}

rw::Clump* RwCanvas::ClumpCreate(rw::World* world)
{
	rw::Clump* clump;
	rw::StreamFile in;

	rw::Image::setSearchPath("./files/clump/");
	const char* filename = "files/clump.dff";
	if (in.open(filename, "rb") == NULL) {
		printf("couldn't open file\n");
		return nil;
	}
	if (!rw::findChunk(&in, rw::ID_CLUMP, NULL, NULL))
		return nil;
	clump = rw::Clump::streamRead(&in);
	in.close();
	if (clump == nil)
		return nil;

	rw::Frame* frame = clump->getFrame();
	rw::V3d pos = { 0.0f, 0.0f, 8.0f };
	frame->translate(&pos, rw::COMBINEREPLACE);
	World->addClump(clump);
	return clump;
}

void RwCanvas::ClumpRotate(rw::Clump* clump, rw::Camera* camera, float xAngle, float yAngle)
{
	rw::Matrix* cameraMatrix = &camera->getFrame()->matrix;
	rw::Frame* clumpFrame = clump->getFrame();
	rw::V3d pos = clumpFrame->matrix.pos;

	pos = rw::scale(pos, -1.0f);
	clumpFrame->translate(&pos, rw::COMBINEPOSTCONCAT);

	clumpFrame->rotate(&cameraMatrix->up, xAngle, rw::COMBINEPOSTCONCAT);
	clumpFrame->rotate(&cameraMatrix->right, yAngle, rw::COMBINEPOSTCONCAT);

	pos = rw::scale(pos, -1.0f);
	clumpFrame->translate(&pos, rw::COMBINEPOSTCONCAT);
}

void RwCanvas::ClumpTranslate(rw::Clump* clump, rw::Camera* camera, float xDelta, float yDelta)
{
	rw::Matrix* cameraMatrix = &camera->getFrame()->matrix;
	rw::Frame* clumpFrame = clump->getFrame();

	rw::V3d deltaX = rw::scale(cameraMatrix->right, xDelta);
	rw::V3d deltaZ = rw::scale(cameraMatrix->at, yDelta);
	rw::V3d delta = rw::add(deltaX, deltaZ);

	clumpFrame->translate(&delta, rw::COMBINEPOSTCONCAT);
}

void RwCanvas::ClumpSetPosition(rw::Clump* clump, rw::V3d* position)
{
	clump->getFrame()->translate(position, rw::COMBINEREPLACE);
}

bool RwCanvas::Initialize3D(void)
{
	Charset = rw::Charset::create(&ForegroundColor, &BackgroundColor);

	World = CreateWorld();

	CamerasCreate(World);
	LightsCreate(World);

	Clump = ClumpCreate(World);

	rw::SetRenderState(rw::CULLMODE, rw::CULLBACK);
	rw::SetRenderState(rw::ZTESTENABLE, 1);
	rw::SetRenderState(rw::ZWRITEENABLE, 1);

	// ImGui_ImplRW_Init();
	// ImGui::StyleColorsClassic();

	rw::Rect r;
	r.x = 0;
	r.y = 0;
	r.w = 1280; // sk::globals.width;
	r.h = 800; // sk::globals.height;
	CameraSizeUpdate(&r, 0.5f, 4.0f / 3.0f);

	return true;
}

void RwCanvas::DestroyLight(rw::Light* light, rw::World* world)
{
	world->removeLight(light);
	rw::Frame* frame = light->getFrame();
	if (frame) {
		light->setFrame(nil);
		frame->destroy();
	}
	light->destroy();
}

void RwCanvas::Terminate3D(void)
{
	if (Clump) {
		World->removeClump(Clump);
		Clump->destroy();
		Clump = nil;
	}

	FORLIST(lnk, World->globalLights) {
		rw::Light* light = rw::Light::fromWorld(lnk);
		DestroyLight(light, World);
	}
	FORLIST(lnk, World->localLights) {
		rw::Light* light = rw::Light::fromWorld(lnk);
		DestroyLight(light, World);
	}

	CamerasDestroy(World);

	if (World) {
		World->destroy();
		World = nil;
	}

	if (Charset) {
		Charset->destroy();
		Charset = nil;
	}
}

void RwCanvas::DisplayOnScreenInfo(void)
{
	char str[256];
	sprintf(str, "View window (%.2f, %.2f)", SubCameraData.viewWindow.x, SubCameraData.viewWindow.y);
	Charset->print(str, 100, 100, 0);
	sprintf(str, "View offset (%.2f, %.2f)", SubCameraData.offset.x, SubCameraData.offset.y);
	Charset->print(str, 100, 120, 0);
}

void RwCanvas::ResetCameraAndClump(void)
{
	SubCameraData.nearClipPlane = 0.3f;
	SubCameraData.farClipPlane = 5.0f;
	SubCameraData.projection = rw::Camera::PERSPECTIVE;
	SubCameraData.offset.x = 0.0f;
	SubCameraData.offset.y = 0.0f;
	SubCameraData.viewWindow.x = 0.5f;
	SubCameraData.viewWindow.y = 0.38f;
	CameraSetData(&SubCameraData, ALL);
	ProjectionIndex = 0;

	rw::V3d position = { 3.0f, 0.0f, 8.0f };
	rw::V3d point = { 0.0f, 0.0f, 8.0f };
	ViewerSetPosition(SubCameraData.camera, &position);
	ViewerRotate(SubCamera, -90.0f, 0.0f);

	ClumpSetPosition(Clump, &point);
}

void RwCanvas::Gui(void)
{
	// static bool showCameraWindow = true;
	// ImGui::Begin("Camera", &showCameraWindow);

	// ImGui::RadioButton("Main camera", &CameraSelected, 0);
	// ImGui::RadioButton("Sub camera", &CameraSelected, 1);

	// if(ImGui::RadioButton("Perspective", &ProjectionIndex, 0))
	// 	ProjectionCallback();
	// if(ImGui::RadioButton("Parallel", &ProjectionIndex, 1))
	// 	ProjectionCallback();

	// if(ImGui::SliderFloat("Near clip-plane", &SubCameraData.nearClipPlane, 0.1f, SubCameraData.farClipPlane-0.1f))
	// 	ClipPlaneCallback();
	// if(ImGui::SliderFloat("Far clip-plane", &SubCameraData.farClipPlane, SubCameraData.nearClipPlane+0.1f, 20.0f))
	// 	ClipPlaneCallback();

	// if(ImGui::Button("Reset"))
	// 	ResetCameraAndClump();

	// ImGui::End();
}

void RwCanvas::MainCameraRender(rw::Camera* camera)
{
	RenderTextureCamera(&BackgroundColorSub, rw::Camera::CLEARIMAGE | rw::Camera::CLEARZ, World);

	camera->clear(&BackgroundColor, rw::Camera::CLEARIMAGE | rw::Camera::CLEARZ);

	camera->beginUpdate();

	// ImGui_ImplRW_NewFrame(TimeDelta);

	World->render();

	DrawCameraViewplaneTexture(&SubCameraData);
	DrawCameraFrustum(&SubCameraData);

	DisplayOnScreenInfo();

	Gui();
	// ImGui::EndFrame();
	// ImGui::Render();

	// ImGui_ImplRW_RenderDrawLists(ImGui::GetDrawData());

	camera->endUpdate();

	RenderSubCamera(&BackgroundColorSub, rw::Camera::CLEARIMAGE | rw::Camera::CLEARZ, World);
}

void RwCanvas::SubCameraRender(rw::Camera* camera)
{
	camera->clear(&BackgroundColor, rw::Camera::CLEARIMAGE | rw::Camera::CLEARZ);

	camera->beginUpdate();

	// ImGui_ImplRW_NewFrame(TimeDelta);

	World->render();

	DisplayOnScreenInfo();

	Gui();
	// ImGui::EndFrame();
	// ImGui::Render();

	// ImGui_ImplRW_RenderDrawLists(ImGui::GetDrawData());

	camera->endUpdate();
}

void RwCanvas::RenderAll(void)
{
	rw::Camera* camera;

	SubCameraMiniViewSelect(CameraSelected == 0);

	switch (CameraSelected) {
	default:
	case 0:
		camera = MainCamera;
		MainCameraRender(camera);
		break;
	case 1:
		camera = SubCamera;
		SubCameraRender(camera);
		break;
	}
	camera->showRaster(0);
}

void RwCanvas::Idle(float timeDelta)
{
	TimeDelta = timeDelta;
	RenderAll();
}

void RwCanvas::MouseMove(int MouseDeltaX, int MouseDeltaY)
{
	if (RotateClump)
		ClumpRotate(Clump, MainCamera, MouseDeltaX, -MouseDeltaY);
	if (TranslateClump)
		ClumpTranslate(Clump, MainCamera, -MouseDeltaX * 0.01f, -MouseDeltaY * 0.1f);
	if (RotateCamera)
		ViewerRotate(SubCamera, -MouseDeltaX * 0.1f, MouseDeltaY * 0.1f);
	if (TranslateCamera)
		ViewerTranslate(SubCamera, -MouseDeltaX * 0.01f, -MouseDeltaY * 0.01f);
	if (ViewXWindow)
		ChangeViewWindow(-MouseDeltaY * 0.01f, 0.0f);
	if (ViewYWindow)
		ChangeViewWindow(0.0f, -MouseDeltaY * 0.01f);
	if (ViewXOffset)
		ChangeViewOffset(-MouseDeltaY * 0.01f, 0.0f);
	if (ViewYOffset)
		ChangeViewOffset(0.0f, -MouseDeltaY * 0.01f);
}

void
RwCanvas::CameraQueryData(CameraData* data, CameraDataType type, rw::Camera* camera)
{
	data->camera = camera;
	if (type & FARCLIPPLANE) data->farClipPlane = camera->farPlane;
	if (type & NEARCLIPPLANE) data->nearClipPlane = camera->nearPlane;
	if (type & PROJECTION) data->projection = camera->projection;
	if (type & OFFSET) data->offset = camera->viewOffset;
	if (type & VIEWWINDOW) data->viewWindow = camera->viewWindow;
	if (type & MATRIX) data->matrix = &camera->getFrame()->matrix;
}

void
RwCanvas::CameraSetData(CameraData* data, CameraDataType type)
{
	if (type & FARCLIPPLANE) data->camera->setFarPlane(data->farClipPlane);
	if (type & NEARCLIPPLANE) data->camera->setNearPlane(data->nearClipPlane);
	if (type & PROJECTION) data->camera->setProjection(data->projection);
	if (type & OFFSET) data->camera->setViewOffset(&data->offset);
	if (type & VIEWWINDOW) data->camera->setViewWindow(&data->viewWindow);
}

void
RwCanvas::ProjectionCallback(void)
{
	if (ProjectionIndex == 0)
		SubCameraData.projection = rw::Camera::PERSPECTIVE;
	else
		SubCameraData.projection = rw::Camera::PARALLEL;
	CameraSetData(&SubCameraData, PROJECTION);
}

void
RwCanvas::ClipPlaneCallback(void)
{
	CameraSetData(&SubCameraData, (CameraDataType)(NEARCLIPPLANE | FARCLIPPLANE));
}

void
RwCanvas::ChangeViewOffset(float deltaX, float deltaY)
{
	SubCameraData.offset.x += deltaX;
	SubCameraData.offset.y += deltaY;
	if (SubCameraData.offset.x > 5.0f)
		SubCameraData.offset.x = 5.0f;
	if (SubCameraData.offset.x < -5.0f)
		SubCameraData.offset.x = -5.0f;
	if (SubCameraData.offset.y > 5.0f)
		SubCameraData.offset.y = 5.0f;
	if (SubCameraData.offset.y < -5.0f)
		SubCameraData.offset.y = -5.0f;
	CameraSetData(&SubCameraData, OFFSET);
}

void
RwCanvas::ChangeViewWindow(float deltaX, float deltaY)
{
	SubCameraData.viewWindow.x += deltaX;
	SubCameraData.viewWindow.y += deltaY;
	if (SubCameraData.viewWindow.x > 5.0f)
		SubCameraData.viewWindow.x = 5.0f;
	if (SubCameraData.viewWindow.x < 0.01f)
		SubCameraData.viewWindow.x = 0.01f;
	if (SubCameraData.viewWindow.y > 5.0f)
		SubCameraData.viewWindow.y = 5.0f;
	if (SubCameraData.viewWindow.y < 0.01f)
		SubCameraData.viewWindow.y = 0.01f;
	CameraSetData(&SubCameraData, VIEWWINDOW);
}

void
RwCanvas::CamerasCreate(rw::World* world)
{
	rw::V3d offset = { 3.0f, 0.0f, 8.0f };
	float rotate = -90.0f;

	SubCamera = ViewerCreate(world);
	ViewerMove(SubCamera, &offset);
	ViewerRotate(SubCamera, rotate, 0.0f);

	MainCamera = ViewerCreate(world);

	CameraQueryData(&SubCameraData, ALL, SubCamera);

	SubCameraData.nearClipPlane = 0.3f;
	CameraSetData(&SubCameraData, NEARCLIPPLANE);

	SubCameraData.farClipPlane = 5.0f;
	CameraSetData(&SubCameraData, FARCLIPPLANE);

	CameraTexture.camera = SubCamera;
	CameraTextureInit(&CameraTexture);

	SubCameraData.cameraTexture = &CameraTexture;


	SubCameraMainCameraSubRaster = rw::Raster::create(0, 0, 0, rw::Raster::CAMERA);
	SubCameraMainCameraSubZRaster = rw::Raster::create(0, 0, 0, rw::Raster::ZBUFFER);
}

void
RwCanvas::CamerasDestroy(rw::World* world)
{
	SubCameraMiniViewSelect(false);

	if (MainCamera) {
		ViewerDestroy(MainCamera, world);
		MainCamera = nil;
	}

	if (SubCamera) {
		ViewerDestroy(SubCamera, world);
		SubCamera = nil;
	}

	CameraTextureTerm(&CameraTexture);

	if (SubCameraMainCameraSubRaster) {
		SubCameraMainCameraSubRaster->destroy();
		SubCameraMainCameraSubRaster = nil;
	}

	if (SubCameraMainCameraSubZRaster) {
		SubCameraMainCameraSubZRaster->destroy();
		SubCameraMainCameraSubZRaster = nil;
	}

	RasterInit = false;
}

void RwCanvas::UpdateSubRaster(rw::Camera* camera, rw::Rect* rect)
{
	rw::Rect subRect;

	subRect.x = rect->w * 0.75f;
	subRect.y = 0;

	subRect.w = rect->w * 0.25f;
	subRect.h = rect->h * 0.25f;

	SubCameraMainCameraSubRaster->subRaster(camera->frameBuffer, &subRect);
	SubCameraMainCameraSubZRaster->subRaster(camera->zBuffer, &subRect);
}

void
RwCanvas::CameraSizeUpdate(rw::Rect* rect, float viewWindow, float aspectRatio)
{
	if (MainCamera == nil)
		return;

	sk::CameraSize(MainCamera, rect, viewWindow, aspectRatio);

	UpdateSubRaster(MainCamera, rect);

	if (RasterInit)
		SubCameraMiniViewSelect(false);

	sk::CameraSize(SubCamera, rect, viewWindow, aspectRatio);

	SubCameraRaster = SubCamera->frameBuffer;
	SubCameraZRaster = SubCamera->zBuffer;

	RasterInit = true;
	SubCameraMiniViewSelect(CameraSelected == 0);

	CameraQueryData(&SubCameraData, VIEWWINDOW, SubCamera);
}

void
RwCanvas::RenderSubCamera(rw::RGBA* backgroundColor, rw::int32 clearMode, rw::World* world)
{
	SubCamera->clear(backgroundColor, clearMode);
	SubCamera->beginUpdate();
	world->render();
	SubCamera->endUpdate();
}

void
RwCanvas::RenderTextureCamera(rw::RGBA* foregroundColor, rw::int32 clearMode, rw::World* world)
{
	rw::Raster* saveRaster, * saveZRaster;

	saveRaster = CameraTexture.camera->frameBuffer;
	saveZRaster = CameraTexture.camera->zBuffer;
	CameraTexture.camera->frameBuffer = CameraTexture.raster;
	CameraTexture.camera->zBuffer = CameraTexture.zRaster;

	CameraTexture.camera->clear(foregroundColor, clearMode);
	CameraTexture.camera->beginUpdate();
	world->render();
	CameraTexture.camera->endUpdate();


	CameraTexture.camera->frameBuffer = saveRaster;
	CameraTexture.camera->zBuffer = saveZRaster;
}

void
RwCanvas::SubCameraMiniViewSelect(bool select)
{
	if (select) {
		SubCamera->frameBuffer = SubCameraMainCameraSubRaster;
		SubCamera->zBuffer = SubCameraMainCameraSubZRaster;
	}
	else {
		SubCamera->frameBuffer = SubCameraRaster;
		SubCamera->zBuffer = SubCameraZRaster;
	}
}



void
RwCanvas::CameraTextureInit(TextureCamera* ct)
{
	ct->raster = rw::Raster::create(TEXSIZE, TEXSIZE, 0, rw::Raster::CAMERATEXTURE);
	assert(ct->raster);
	ct->zRaster = rw::Raster::create(TEXSIZE, TEXSIZE, 0, rw::Raster::ZBUFFER);
	assert(ct->zRaster);

	ct->texture = rw::Texture::create(ct->raster);
	ct->texture->setFilter(rw::Texture::FilterMode::LINEAR);
}

void
RwCanvas::CameraTextureTerm(TextureCamera* ct)
{
	if (ct->raster) {
		ct->raster->destroy();
		ct->raster = nil;
	}

	if (ct->zRaster) {
		ct->zRaster->destroy();
		ct->zRaster = nil;
	}

	if (ct->texture) {
		ct->texture->raster = nil;
		ct->texture->destroy();
		ct->texture = nil;
	}
}



void
RwCanvas::DrawCameraFrustum(CameraData* c)
{
	rw::RGBA yellow = { 255, 255, 0, 64 };
	rw::RGBA red = { 255, 0, 0, 255 };
	rw::RWDEVICE::Im3DVertex frustum[13];
	// lines
	rw::uint16 indicesL[] = {
		1,  2,  2,  3,  3,  4,  4,  1,
		5,  6,  6,  7,  7,  8,  8,  5,
		9, 10, 10, 11, 11, 12, 12,  9,
		5,  9,  6, 10,  7, 11,  8, 12,
		0,  0
	};
	// triangles
	rw::uint16 indicesT[] = {
		 5,  6, 10,
		10,  9,  5,
		 6,  7, 11,
		11, 10,  6,
		 7,  8, 12,
		12, 11,  7,
		 8,  5,  9,
		 9, 12,  8,

		 7,  6,  5,
		 5,  8,  7,
		 9, 10, 11,
		11, 12,  9
	};
	float signs[4][2] = {
		{  1,  1 },
		{ -1,  1 },
		{ -1, -1 },
		{  1, -1 }
	};

	float depth[3];
	depth[0] = 1.0f;	// view window
	depth[1] = c->nearClipPlane;
	depth[2] = c->farClipPlane;

	int k = 0;
	frustum[k].setX(c->offset.x);
	frustum[k].setY(c->offset.y);
	frustum[k].setZ(0.0f);
	k++;

	for (int i = 0; i < 3; i++)	// depths
		for (int j = 0; j < 4; j++) {	// planes
			if (c->projection == rw::Camera::PERSPECTIVE) {
				frustum[k].setX(-c->offset.x + depth[i] * (signs[j][0] * c->viewWindow.x + c->offset.x));
				frustum[k].setY(c->offset.y + depth[i] * (signs[j][1] * c->viewWindow.y - c->offset.y));
				frustum[k].setZ(depth[i]);
			}
			else {
				frustum[k].setX(-c->offset.x + signs[j][0] * c->viewWindow.x + depth[i] * c->offset.x);
				frustum[k].setY(c->offset.y + signs[j][1] * c->viewWindow.y - depth[i] * c->offset.y);
				frustum[k].setZ(depth[i]);
			}
			k++;
		}

	for (int i = 0; i < 5; i++)
		frustum[i].setColor(red.red, red.green, red.blue, red.alpha);
	for (int i = 5; i < 13; i++)
		frustum[i].setColor(yellow.red, yellow.green, yellow.blue, 255);

	rw::SetRenderStatePtr(rw::TEXTURERASTER, nil);

	rw::im3d::Transform(frustum, 13, c->camera->getFrame()->getLTM(), rw::im3d::ALLOPAQUE);
	rw::im3d::RenderIndexedPrimitive(rw::PRIMTYPELINELIST, indicesL, 34);
	rw::im3d::End();

	for (int i = 5; i < 13; i++)
		frustum[i].setColor(yellow.red, yellow.green, yellow.blue, yellow.alpha);

	rw::SetRenderState(rw::VERTEXALPHA, 1);

	rw::im3d::Transform(frustum, 13, c->camera->getFrame()->getLTM(), rw::im3d::ALLOPAQUE);
	rw::im3d::RenderIndexedPrimitive(rw::PRIMTYPETRILIST, indicesT, 36);
	rw::im3d::End();
}

void
RwCanvas::DrawCameraViewplaneTexture(CameraData* c)
{
	rw::RGBA white = { 255, 255, 255, 255 };
	rw::RWDEVICE::Im3DVertex frustum[4];
	rw::uint16 indicesV[] = {
		2, 1, 0,
		0, 3, 2,
		0, 1, 2,
		2, 3, 0
	};
	float uvValues[4][2] = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f }
	};
	float signs[4][2] = {
		{  1,  1 },
		{ -1,  1 },
		{ -1, -1 },
		{  1, -1 }
	};

	for (int j = 0; j < 4; j++) {
		frustum[j].setX(signs[j][0] * c->viewWindow.x);
		frustum[j].setY(signs[j][1] * c->viewWindow.y);
		frustum[j].setZ(1.0f);
	}
	for (int i = 0; i < 4; i++) {
		frustum[i].setColor(white.red, white.green, white.blue, white.alpha);
		frustum[i].setU(uvValues[i][0]);
		frustum[i].setV(uvValues[i][1]);
	}

	rw::SetRenderState(rw::VERTEXALPHA, 1);
	rw::SetRenderStatePtr(rw::TEXTURERASTER, c->cameraTexture->texture->raster);

	rw::im3d::Transform(frustum, 4, c->camera->getFrame()->getLTM(), rw::im3d::VERTEXUV);
	rw::im3d::RenderIndexedPrimitive(rw::PRIMTYPETRILIST, indicesV, 12);
	rw::im3d::End();
}
