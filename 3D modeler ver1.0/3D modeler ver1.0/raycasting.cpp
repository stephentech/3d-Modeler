
#include <raycasting.h>
#include <common.h>

extern Camera *currentSelectedCamera;

void computeRay3D(LPDIRECT3DDEVICE9 d3ddev, int x, int y, D3DXVECTOR3 *Near, D3DXVECTOR3 *dir )
{
	D3DVIEWPORT9 Viewport;
	d3ddev->GetViewport( &Viewport );

	D3DXMATRIX World;
	d3ddev->GetTransform(D3DTS_WORLD ,&World);

	D3DXVECTOR3 Out;
	Near->x = x;	Near->y = y;	Near->z = 0;
	dir->x = x;		dir->y = y;		dir->z = currentSelectedCamera->nearPlane;
	
	D3DXVec3Unproject(&Out, Near, &Viewport, &(currentSelectedCamera->matProjection), &(currentSelectedCamera->matView), &World);
	Near->x = Out.x;	Near->y = Out.y;	Near->z = Out.z;

	D3DXVec3Unproject(&Out, dir, &Viewport, &(currentSelectedCamera->matProjection), &(currentSelectedCamera->matView), &World);
	dir->x = Out.x;		dir->y = Out.y;		dir->z = Out.z;
	
	dir->x -= Near->x;		dir->y -= Near->y;		dir->z -= Near->z;
}

bool computeBoundingBox(Object *ptr, D3DXVECTOR3 *lowerVertex, D3DXVECTOR3 *higherVertex)
{
		lowerVertex->x = ptr->vert[0].x;
		lowerVertex->y = ptr->vert[0].y;
		lowerVertex->z = ptr->vert[0].z;

		higherVertex->x = ptr->vert[0].x;
		higherVertex->y = ptr->vert[0].y;
		higherVertex->z = ptr->vert[0].z;
		
		for(int i=1; i<ptr->numofVertices; i++)
		{
			if( ptr->vert[i].x < lowerVertex->x ) lowerVertex->x = ptr->vert[i].x;
			if( ptr->vert[i].y < lowerVertex->y ) lowerVertex->y = ptr->vert[i].y;
			if( ptr->vert[i].z < lowerVertex->z ) lowerVertex->z = ptr->vert[i].z;

			if( ptr->vert[i].x > higherVertex->x ) higherVertex->x = ptr->vert[i].x;
			if( ptr->vert[i].y > higherVertex->y ) higherVertex->y = ptr->vert[i].y;
			if( ptr->vert[i].z > higherVertex->z ) higherVertex->z = ptr->vert[i].z;
		}

	return 1;
}