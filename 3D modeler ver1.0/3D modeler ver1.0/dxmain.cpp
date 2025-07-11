
#include <dxmain.h>
#include <d3d9.h>
#include <dxDraw.h>
#include <common.h>
#include <camera.h>
#include <3DSLoader.h>
#include <Texture.h>

// include the Direct3D Library file
#pragma comment (lib, "d3dx9.lib")

LPDIRECT3D9 d3d;			 // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class

bool shiftFlag = 0;
bool ctrlFlag = 0;
char gridFlag = 1;
char ObjectFlag = 1;
char pivotFlag = 0;
char focusFlag = 0;

int xPosDown, yPosDown;
int xPosUp, yPosUp;

void dxInit(HWND hWnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);		// create the Direct3D interface
	
	D3DDISPLAYMODE d3ddm;
	d3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

    D3DPRESENT_PARAMETERS d3dpp;				// create a struct to hold various device information
    ZeroMemory(&d3dpp, sizeof(d3dpp));			// clear out the struct for use
   
	d3dpp.Windowed = TRUE;						// program windowed, not fullscreen
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// discard old frames
    d3dpp.hDeviceWindow = hWnd;					// set the window to be used by Direct3D
	d3dpp.BackBufferFormat = d3ddm.Format;		// set the back buffer format to 32-bit
	d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.BackBufferWidth = WINDOW_WIDTH;		// set the width of the buffer
    d3dpp.BackBufferHeight = WINDOW_HEIGHT;		// set the height of the buffer

    // create a device class using this information and information from the d3dpp stuct
    if(FAILED ( d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev) ) )
	{
		MessageBox(hWnd,L"Error in creating d3d device", L"Error",MB_OK);
		exit(0);
	}

	CreateCamera();
	createNewObject(d3ddev,'G');
	d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
    return;
}

void dxRender()
{
	// clear the window to a deep blue
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET,  BACKGROUND_COLOR, 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, BACKGROUND_COLOR, 1.0f, 0);

	DrawObject(d3ddev);

    d3ddev->Present(NULL, NULL, NULL, NULL);    // displays the created frame
    return;
}

void dxCleanup()
{
	bufferCleanup();
	cameraCleanup();
	if(d3ddev)	
		d3ddev->Release();		// close and release the 3D device
    if(d3d)
		d3d->Release();			// close and release Direct3D
    return;
}

void dxLeftClickDown(int x, int y)
{
	xPosDown = x;
	yPosDown = y;

	selectObject(d3ddev, x, y);
}

void dxLeftClickUp(int x, int y)
{
	xPosUp = x;
	yPosUp = y;

	if(isObjectSelected())
	{
		D3DXVECTOR3 vec, Orig;
		getObjectPivot(&Orig);
		vec = Orig;
		projectCameraVector(d3ddev, &vec);

		vec.x = vec.x - (xPosUp - xPosDown);
		vec.y = vec.y - (yPosUp - yPosDown);

		unProjectCameraVector(d3ddev, &vec);
		switch(ObjectFlag)
		{
		case 1:
			if(pivotFlag)
				translatePivot(NULL, (Orig.x - vec.x), (Orig.y - vec.y), (Orig.z - vec.z));
			else
				translateObject(NULL, (Orig.x - vec.x), (Orig.y - vec.y), (Orig.z - vec.z));
			break;

		case 2:
			rotateObject(NULL, ( Orig.x - vec.x ), (Orig.y - vec.y), (Orig.z - vec.z));
			break;

		case 3:
			if(Orig.x - vec.x > 0 )
				scaleObject(NULL, 1.2f, 1.2f, 1.2f);
			else 
				scaleObject(NULL, 0.8f, 0.8f, 0.8f);
			break;
		}
	}
}

void dxRightClickDown(int x, int y)
{
	xPosDown = x;
	yPosDown = y;
}

void dxRightClickUp(int x, int y)
{
	xPosUp = x;
	yPosUp = y;

	D3DXVECTOR3 vec, Orig;
	getCameraFocus(&Orig);
	vec = Orig;
	projectCameraVector(d3ddev, &vec);

	vec.x = vec.x - (xPosUp - xPosDown);
	vec.y = vec.y - (yPosUp - yPosDown);

	unProjectCameraVector(d3ddev, &vec);

	if(focusFlag)
		translateFocus(NULL, (Orig.x - vec.x), (Orig.y - vec.y), (Orig.z - vec.z));
	else
		translateCamera(NULL, (Orig.x - vec.x), (Orig.y - vec.y), (Orig.z - vec.z));
			
}

void dxKeyDown(char key)
{
	static char CamObjFlag = 0;
	switch(key)
	{
		case 0x31:	// 1- key
			createNewObject(d3ddev,'1');
			break;

		case 0x32:	// 2- key
			createNewObject(d3ddev,'2');
			break;

		case 0x33:	// 3- key
			createNewObject(d3ddev,'3');
			break;

		case 0x34:	// 4- key
			createNewObject(d3ddev,'4');
			break;

		case 0x35:	// 5- key
			createNewObject(d3ddev,'5');
			break;

		case 0x36:	// 6- key
			createNewObject(d3ddev,'6');
			break;

		case 0x37:	// 7- key
			createNewObject(d3ddev,'7');
			break;

		case 0x38:	// 8- key
			createNewObject(d3ddev,'8');
			break;

		case 0x39:	// 9- key
			createNewObject(d3ddev,'9');
			break;

		case 0x46:
			focusFlag++;
			focusFlag %= 2;
			break;

		case 0x4F:	// O- key
			Load3DS("3DS_files\\sphere.3DS");
			updateAllVertexBuffer(d3ddev);
			updateAllIndexBuffer(d3ddev);
			break;

		case 0x47:	// G-key
			// toggle grid status
			gridFlag++;
			gridFlag %= 2;
			gridStatus( gridFlag );
			break;

		case 0x50:	/// P- key
			pivotFlag++;
			pivotFlag %= 2;
			break;

		case 0x53:	/// S- key
			if(shiftFlag)
				scaleObject(NULL, 0.5f, 0.5f, 0.5f);
			else 
				scaleObject(NULL, 2.0f, 2.0f, 2.0f);
			break;

		case VK_F1:
			d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			break;
		
		case VK_F2:
			d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			break;

		case VK_F4:
			calculateTextureCoordinates( NULL );
			applyTexture( d3ddev ,L"texture\\texture.jpg");
			UpdateVertexInfo(NULL, NULL);
			break;

		case VK_F3:
			calculateTextureCoordinates( NULL );
			applyTexture( d3ddev ,L"texture\\texture1.jpg");
			UpdateVertexInfo(NULL, NULL);
			break;

		case VK_F5:
			ObjectFlag = 1;
			break;

		case VK_F6:
			ObjectFlag = 2;
			break;

		case VK_F7:
			ObjectFlag = 3;
			break;

		case VK_ESCAPE:
			bufferCleanup();
			createNewObject(d3ddev, 'G');
			break;

		case VK_DELETE:
			if(isObjectSelected())
				deleteSelectObject();
			break;

		case VK_SPACE:
			selectNextObject();
			break;

		case VK_SHIFT:
			shiftFlag = 1;
			break;

		case VK_CONTROL:
			ctrlFlag = 1;
			break;

		case VK_RIGHT:
			if(CamObjFlag)
			{
				if(ctrlFlag)
					translateObject(NULL, 5.0f, 0.0f, 0.0f);
				else
					rotateObject(NULL, 0.0f, -5.0f, 0.0f);
			}
			else
			{
				if(ctrlFlag)
					translateCamera(NULL, 5.0f, 0.0f, 0.0f);
				else 
					rotateCamera(NULL, 0.0f, 5.0f, 0.0f);
			}
			break;

		case VK_LEFT:
			if(CamObjFlag)
			{
				if(ctrlFlag)
					translateObject(NULL, -5.0f, 0.0f, 0.0f);
				else
					rotateObject(NULL, 0.0f, 5.0f, 0.0f);
			}
			else
			{
				if(ctrlFlag)
					translateCamera(NULL, -5.0f, 0.0f, 0.0f);
				else
					rotateCamera(NULL, 0.0f, -5.0f, 0.0f);
			}
			break;

		case VK_UP:
			if(CamObjFlag)
			{
				if(ctrlFlag)
					translateObject(NULL, 0.0f, 0.0f, 5.0f);
				else
					rotateObject(NULL, 5.0f, 0.0f, 0.0f);
			}
			else
			{
				if(ctrlFlag)
					translateCamera(NULL, 0.0f, 0.0f, 5.0f);
				else
					rotateCamera(NULL, -5.0f, 0.0f, 0.0f);
			}
			break;

		case VK_DOWN:
			if(CamObjFlag)
			{
				if(ctrlFlag)
					translateObject(NULL, 0.0f, 0.0f, -5.0f);
				else
					rotateObject(NULL, -5.0f, 0.0f, 0.0f);
			}
			else
			{
				if(ctrlFlag)
					translateCamera(NULL, 0.0f, 0.0f, -5.0f);
				else
					rotateCamera(NULL, 5.0f, 0.0f, 0.0f);
			}
			break;

		case VK_HOME:
			if(CamObjFlag)
			{
				if(ctrlFlag)
					translateObject(NULL, 0.0f, 5.0f, 0.0f);
				else
					rotateObject(NULL, 0.0f, 0.0f, 5.0f);
			}
			else
			{
				if(ctrlFlag)
					translateCamera(NULL, 0.0f, 5.0f, 0.0f);
				else
					rotateCamera(NULL, 0.0f, 0.0f, 5.0f);
			}
			break;

		case VK_END:
			if(CamObjFlag)
			{
				if(ctrlFlag)
					translateObject(NULL, 0.0f, -5.0f, 0.0f);
				else
					rotateObject(NULL, 0.0f, 0.0f, -5.0f);
			}
			else
			{
				if(ctrlFlag)
					translateCamera(NULL, 0.0f, -5.0f, 0.0f);
				else
					rotateCamera(NULL, 0.0f, 0.0f, -5.0f);
			}
			break;

		case VK_TAB:// tab
			CamObjFlag++;
			CamObjFlag %= 2;
			break;
	}

}

void dxKeyUp(char key)
{
	switch(key)
	{
		case VK_SHIFT:
			shiftFlag = 0;
			break;

		case VK_CONTROL:
			ctrlFlag = 0;
			break;
	}
}
