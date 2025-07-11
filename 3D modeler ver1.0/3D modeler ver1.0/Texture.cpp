
#include <Texture.h>
#include <raycasting.h>

extern Object *currentSelectedObject;
extern Object *startObject;

void calculateTextureCoordinates(Object *obj)
{
	if(obj == NULL)
		obj = currentSelectedObject;
	if(obj == NULL)
		return;

	D3DXVECTOR3 low, high;
	computeBoundingBox(obj, &low, &high);

	for(int i=0; i< obj->numofVertices; i++)
	{
		obj->vert[i].u = 1 - ( (obj->vert[i].x - low.x) / (high.x - low.x) );
		obj->vert[i].v = 1 - ( (obj->vert[i].y - low.y) / (high.y - low.y) );
	}
}

bool applyTexture(LPDIRECT3DDEVICE9 d3ddev, LPCWSTR fileName)
{
	if(currentSelectedObject->texture)
		deleteTexture();

	if( D3DXCreateTextureFromFile(d3ddev, fileName, &(currentSelectedObject->texture) ) != D3D_OK)
	{
		MessageBox(NULL,L"Failed to load texture", L"Error",MB_OK);
		return 0;
	}

	currentSelectedObject->color = D3DCOLOR_XRGB (0xAA,0xAA,0xAA);
	return 1;
}

void deleteTexture()
{
	if(currentSelectedObject == NULL)
		return;
	currentSelectedObject->texture->Release();
	currentSelectedObject->texture = NULL;
}