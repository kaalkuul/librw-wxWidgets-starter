#pragma once

#include "wxRwCanvas.h"

struct TextureCamera
{
	rw::Raster* raster;
	rw::Raster* zRaster;
	rw::Camera* camera;
	rw::Texture* texture;
};

struct CameraData
{
	float farClipPlane;
	float nearClipPlane;
	rw::uint32 projection;
	rw::V2d offset;
	rw::V2d viewWindow;
	rw::Camera* camera;
	TextureCamera* cameraTexture;
	rw::Matrix* matrix;
};

enum CameraDataType
{
	NONE = 0x00,
	FARCLIPPLANE = 0x01,
	NEARCLIPPLANE = 0x02,
	PROJECTION = 0x04,
	OFFSET = 0x08,
	VIEWWINDOW = 0x10,
	MATRIX = 0x20,
	ALL = 0xFF
};

class RwCanvas : public wxRwCanvas
{
    wxDECLARE_EVENT_TABLE();

public:
    RwCanvas(wxWindow* parent);
    virtual ~RwCanvas();

protected:
    virtual wxRwGLContext* InitContext();
    virtual void InitRwResources();
    virtual void Render();

private:
    void OnMouseMoved(wxMouseEvent& event);

	rw::World* CreateWorld(void);
	void LightsCreate(rw::World* world);
	rw::Clump* ClumpCreate(rw::World* world);
	void ClumpRotate(rw::Clump* clump, rw::Camera* camera, float xAngle, float yAngle);
	void ClumpTranslate(rw::Clump* clump, rw::Camera* camera, float xDelta, float yDelta);
	void ClumpSetPosition(rw::Clump* clump, rw::V3d* position);
	bool Initialize3D(void);
	void DestroyLight(rw::Light* light, rw::World* world);
	void Terminate3D(void);
	void DisplayOnScreenInfo(void);
	void ResetCameraAndClump(void);
	void Gui(void);
	void MainCameraRender(rw::Camera* camera);
	void SubCameraRender(rw::Camera* camera);
	void RenderAll(void);
	void Idle(float timeDelta);
	void MouseMove(int mouseposx, int mouseposy);

	void CameraQueryData(CameraData* data, CameraDataType type, rw::Camera* camera);
	void CameraSetData(CameraData* data, CameraDataType type);

	void ChangeViewOffset(float deltaX, float deltaY);
	void ChangeViewWindow(float deltaX, float deltaY);
	void ProjectionCallback(void);
	void ClipPlaneCallback(void);

	void CamerasCreate(rw::World* world);
	void CamerasDestroy(rw::World* world);
	void UpdateSubRaster(rw::Camera* camera, rw::Rect* rect);
	void CameraSizeUpdate(rw::Rect* rect, float viewWindow, float aspectRatio);
	void RenderSubCamera(rw::RGBA* foregroundColor, rw::int32 clearMode, rw::World* world);
	void RenderTextureCamera(rw::RGBA* foregroundColor, rw::int32 clearMode, rw::World* world);
	void SubCameraMiniViewSelect(bool select);

	void CameraTextureInit(TextureCamera* ct);
	void CameraTextureTerm(TextureCamera* ct);
	void DrawCameraFrustum(CameraData* c);
	void DrawCameraViewplaneTexture(CameraData* c);

private:
    wxPoint m_lastMousePos;

	rw::V3d zero = { 0.0f, 0.0f, 0.0f };
	float FOV = 70.0f;

	rw::RGBA ForegroundColor = { 200, 200, 200, 255 };
	rw::RGBA BackgroundColor = { 64, 64, 64, 0 };
	rw::RGBA BackgroundColorSub = { 74, 74, 74, 0 };

	rw::World* World;
	rw::Charset* Charset;

	rw::V3d Xaxis = { 1.0f, 0.0, 0.0f };
	rw::V3d Yaxis = { 0.0f, 1.0, 0.0f };
	rw::V3d Zaxis = { 0.0f, 0.0, 1.0f };

	float TimeDelta;

	rw::Clump* Clump;


	bool RotateClump;
	bool TranslateClump;
	bool RotateCamera;
	bool TranslateCamera;
	bool ViewXWindow;
	bool ViewYWindow;
	bool ViewXOffset;
	bool ViewYOffset;


	rw::Camera* MainCamera;
	rw::Camera* SubCamera;

	rw::Raster* SubCameraRaster;
	rw::Raster* SubCameraZRaster;
	rw::Raster* SubCameraMainCameraSubRaster;
	rw::Raster* SubCameraMainCameraSubZRaster;

	TextureCamera CameraTexture;

	rw::int32 CameraSelected = 0;
	rw::int32 ProjectionIndex = 0;
	bool SubCameraMiniView = true;

	CameraData SubCameraData;

	bool RasterInit = false;
};
