
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <dataStructure.h>

#define FIELD_OF_VIEW 45
#define ASPECT_RATIO 1.33f
#define NORMAL_NEAR_DEPTH 1.0f
#define NORMAL_FAR_DEPTH 5000.0f

#define XY_PLANE 1001
#define XZ_PLANE 1002
#define YZ_PLANE 1003

void setCurrentCamera(LPDIRECT3DDEVICE9 );
void CreateCamera();

void getCameraFocus(D3DXVECTOR3*);

void rotateCamera(Camera*, float, float, float);
void translateCamera(Camera*, float, float, float);
void translateFocus(Camera*, float, float, float);

void cameraCleanup();

void projectCameraVector(LPDIRECT3DDEVICE9, D3DXVECTOR3*);
void unProjectCameraVector(LPDIRECT3DDEVICE9, D3DXVECTOR3*);

#endif