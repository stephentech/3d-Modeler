
#include <camera.h>
#include <common.h>

Camera *startCamera = NULL;
Camera *currentSelectedCamera = NULL;


Camera::Camera()
{
	Eye.x =	300.0f;	Eye.y = 500.0f;	Eye.z = -700.0f;
	At.x =			At.y =			At.z = 0.0f;
	Up.x = 0.0f;	Up.y = 1.0f;	Up.z = 0.0f;

	fov			= FIELD_OF_VIEW;
	aspectRatio = ASPECT_RATIO;
	nearPlane	= NORMAL_NEAR_DEPTH;
	farPlane	= NORMAL_FAR_DEPTH;

	width  = WINDOW_WIDTH;
	height = WINDOW_HEIGHT;
}

void setCurrentCamera(LPDIRECT3DDEVICE9 d3ddev)
{
	d3ddev->SetTransform(D3DTS_VIEW, &(currentSelectedCamera->matView));
	d3ddev->SetTransform(D3DTS_PROJECTION, &(currentSelectedCamera->matProjection));	
}

void CreateCamera()
{
	Camera *ptr;
	if(!startCamera)
		startCamera = currentSelectedCamera = ptr = new Camera;
	else
	{
		ptr = startCamera;
		while(ptr->nextCamera) ptr = ptr->nextCamera;
		ptr->nextCamera = new Camera;
		ptr = ptr->nextCamera;
	}

	ptr->nextCamera = NULL;
	ZeroMemory(&(ptr->matView), sizeof(D3DXMATRIX));
	ZeroMemory(&(ptr->matProjection), sizeof(D3DXMATRIX));

	D3DXMatrixLookAtLH(&(ptr->matView), &(ptr->Eye), &(ptr->At), &(ptr->Up) );
	D3DXMatrixPerspectiveFovLH(&(ptr->matProjection), D3DXToRadian(ptr->fov), ptr->aspectRatio, ptr->nearPlane, ptr->farPlane);
}

void getCameraFocus(D3DXVECTOR3 *vec)
{
	vec->x = currentSelectedCamera->At.x;
	vec->y = currentSelectedCamera->At.y;
	vec->z = currentSelectedCamera->At.z;
}

void rotateCamera(Camera *cam, float angleX, float angleY, float angleZ )
{
	D3DXMATRIX rotate, result;
	if (cam == NULL) 
		cam = currentSelectedCamera;
	if (cam == NULL)
		return;
	
	D3DXMatrixRotationYawPitchRoll( &rotate, D3DXToRadian(angleY) , D3DXToRadian(angleX), D3DXToRadian(angleZ) );
	result = rotate * cam->matView;
	cam->matView = result;
}

void translateCamera(Camera *cam, float x, float y, float z)
{
	if (cam == NULL) 
		cam = currentSelectedCamera;

	D3DXMATRIX translate, result;
	D3DXMatrixTranslation( &translate, x, y, z );
	D3DXMatrixMultiply( &result, &translate, &(cam->matView) );
	cam->matView = result;
}

void translateFocus(Camera *cam, float x, float y, float z)
{
	if (cam == NULL) 
		cam = currentSelectedCamera;

	cam->At.x += x;
	cam->At.y += y;	
	cam->At.z += z;

	D3DXMatrixLookAtLH(&(cam->matView), &(cam->Eye), &(cam->At), &(cam->Up) );
	D3DXMatrixPerspectiveFovLH(&(cam->matProjection), D3DXToRadian(cam->fov), cam->aspectRatio, cam->nearPlane, cam->farPlane);
}

void cameraCleanup()
{
	if (!startCamera) return;
	Camera *ptr, *ptr2;
	ptr = ptr2 = startCamera;
	while(ptr)
	{
		ptr = ptr->nextCamera;
		if(ptr2) delete ptr2;
		ptr2 = ptr;
	}
	startCamera = NULL;
	currentSelectedCamera = NULL;
}

void projectCameraVector(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 *vec)
{
	D3DVIEWPORT9 Viewport;
	d3ddev->GetViewport( &Viewport );

	D3DXMATRIX World;
	d3ddev->GetTransform(D3DTS_WORLD ,&World);

	D3DXVECTOR3 Out;
	D3DXVec3Project(&Out, vec, &Viewport, &(currentSelectedCamera->matProjection), &(currentSelectedCamera->matView), &World);

	vec->x = Out.x;
	vec->y = Out.y;
	vec->z = Out.z;
}

void unProjectCameraVector(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 *vec)
{
	D3DVIEWPORT9 Viewport;
	d3ddev->GetViewport( &Viewport );

	D3DXMATRIX World;
	d3ddev->GetTransform(D3DTS_WORLD ,&World);

	D3DXVECTOR3 Out;
	D3DXVec3Unproject(&Out, vec, &Viewport, &(currentSelectedCamera->matProjection), &(currentSelectedCamera->matView), &World);

	vec->x = Out.x;
	vec->y = Out.y;
	vec->z = Out.z;
}