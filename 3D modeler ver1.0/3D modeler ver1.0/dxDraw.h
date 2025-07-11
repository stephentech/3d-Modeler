
#ifndef _DXDRAW_H_
#define _DXDRAW_H_

#include <windows.h>
#include <dataStructure.h>

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define GRID_COLOR D3DCOLOR_XRGB(0x80, 0x80, 0x80)

class Triangle:public Object
{
public:
	/// class functions ...
	Triangle();
	~Triangle();
};

class Grid:public Object
{
public:
	/// class functions ...
	Grid();
	~Grid();
};

void createNewObject(LPDIRECT3DDEVICE9 d3ddev,char name);
void DrawObject(LPDIRECT3DDEVICE9 d3ddev);
void bufferCleanup();

void selectNextObject();
void Select(Object*);
void deSelect();

void rotateObject(Object*, float, float, float);
void translateObject(Object*, float, float, float);
void scaleObject(Object*, float, float, float);
void translatePivot(Object*, float, float, float);

void UpdateVertexInfo( Object*, D3DXMATRIX*);
void updateAllVertexBuffer(LPDIRECT3DDEVICE9);
void updateAllIndexBuffer(LPDIRECT3DDEVICE9);

void selectObject(LPDIRECT3DDEVICE9, int x=0, int y=0);
void deleteSelectObject();

void getObjectPivot(D3DXVECTOR3*);
bool isObjectSelected();

void gridStatus(bool);

#endif
