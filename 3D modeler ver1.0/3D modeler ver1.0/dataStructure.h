
#ifndef _DATASTRUCTURE_H_
#define _DATASTRUCTURE_H_

#include <d3d9.h>
#include <D3DX9Math.h>

struct CUSTOMVERTEX
{
    float x, y, z;	// position coordinate
	DWORD color;	// colour of Vertex
	float u, v;		// texture mapping coordinates
};

struct INDICE
{
	unsigned short int v1, v2, v3;
};

class Object
{
public:
	char name[50];
	BOOL renderFlag;
	int drawPrimitiveType;

	DWORD color;
	int numofVertices;
	int numofIndices;
	int sizeofIndices;

	CUSTOMVERTEX PIVOT;
	CUSTOMVERTEX *vert;
	INDICE *indices;
	
	LPDIRECT3DVERTEXBUFFER9 buffer;
	LPDIRECT3DINDEXBUFFER9 indexBuffer;
	LPDIRECT3DTEXTURE9 texture;

	Object *nextObject;

	Object();
	~Object();
};

class Camera
{
public:
	char name[50];
	float fov, aspectRatio, nearPlane, farPlane;
	int width, height;

	D3DXVECTOR3 Eye, At, Up;
	D3DXMATRIX matView;
	D3DXMATRIX matProjection;
	
	Camera *nextCamera;
	
	Camera();
};


#endif