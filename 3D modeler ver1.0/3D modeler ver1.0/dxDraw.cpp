
#include <dxDraw.h>
#include <raycasting.h>
#include <VTXloader.h>
#include <Camera.h>

Object *startObject = NULL;
Object *currentSelectedObject = NULL;

Object::Object()
{
	nextObject = NULL;
	vert = NULL;
	buffer = NULL;
	indexBuffer = NULL;
	texture = NULL;

	renderFlag = 1;
	numofVertices = 0;
	numofIndices = 0;
	drawPrimitiveType = D3DPT_TRIANGLELIST;
	PIVOT.x = PIVOT.y = PIVOT.z = 0;
	
	color = D3DCOLOR_XRGB (0x00,0x77,0xff);
}

Object::~Object()
{
	if (vert) delete[] vert;
	if (indices) delete[] indices;
	if(buffer)
		buffer->Release();
	if(indexBuffer)
		indexBuffer->Release();
	if(texture)
		 texture->Release();
}

Grid::Grid()
{
	nextObject = NULL;
	vert = NULL;
	buffer = NULL;
	indexBuffer = NULL;
	texture = NULL;

	color = GRID_COLOR;
	renderFlag = 1;

	drawPrimitiveType = D3DPT_LINELIST;
	PIVOT.x = PIVOT.y = PIVOT.z = 0;

	numofVertices = 64;
	vert = new CUSTOMVERTEX[numofVertices];

	for(int i = 0; i < numofVertices/2; i+=2)
	{
		vert[i].x =				vert[i+1].x = (i - numofVertices/4) * 20.0f;
		vert[i].y =				vert[i+1].y = 0.0f;
		vert[i].z = -320.0f;	vert[i+1].z = 280.0f;

		vert[i].color =			vert[i+1].color = GRID_COLOR;
		vert[i].u = 0.0f;		vert[i+1].u = 0.0f;
		vert[i].v = 0.0f;		vert[i+1].v = 0.0f;
	}

	for(int i = numofVertices/2; i<numofVertices; i+=2)
	{
		vert[i].x =	-320.0f;	vert[i+1].x = 280.0f;
		vert[i].y =				vert[i+1].y = 0.0f;
		vert[i].z =				vert[i+1].z = (i - numofVertices/2 - numofVertices/4) * 20.0f;

		vert[i].color =			vert[i+1].color = GRID_COLOR;
		vert[i].u = 0.0f;		vert[i+1].u = 0.0f;
		vert[i].v = 0.0f;		vert[i+1].v = 0.0f;
	}

	numofIndices = 32;
	sizeofIndices = sizeof(INDICE) * 22; 
	indices = new INDICE[22];
	short temp[66] = { 0, 1, 2, 3, 4, 5, 6, 7, 
					8, 9, 10, 11, 12, 13, 14, 15, 
					16, 17, 18, 19, 20, 21, 22, 23,
					24,	25, 26, 27, 28, 29, 30, 31,
					32, 33, 34, 35, 36, 37, 38, 39,
					40,	41, 42, 43, 44, 45, 46, 47,
					48,	49, 50, 51, 52, 53, 54, 55, 
					56,	57, 58, 59, 60, 61, 62, 63, 0, 63 };
	memmove_s(indices, sizeofIndices, temp, sizeof(temp) );
}

Grid::~Grid()
{
	if(vert) delete[] vert;
	if(indices) delete[] indices;
	if(buffer)
		buffer->Release();
	if(indexBuffer)
		indexBuffer->Release();
	if(texture)
		texture->Release();
}

void createNewObject(LPDIRECT3DDEVICE9 d3ddev,char name)
{
	Object *ptr, *newObjPtr;
	switch (name)
	{
	case '1': 
		/// Model Load Sphere
		newObjPtr = new Object();
		strcpy(newObjPtr->name,"Sphere");
		if ( LoadModel_vtx( "sphere", newObjPtr ) == 0 )
		{
			delete newObjPtr;
			return;
		}
		break;

	case '2': 
		/// Model Load Chamfer
		newObjPtr = new Object();
		strcpy(newObjPtr->name,"Chamfer");
		if ( LoadModel_vtx( "Chamfer", newObjPtr ) == 0 )
		{
			delete newObjPtr;
			return;
		}
		break;

	case '3': 
		/// Model Load Cone
		newObjPtr = new Object();
		strcpy(newObjPtr->name,"Cone");
		if ( LoadModel_vtx( "Cone", newObjPtr ) == 0 )
		{
			delete newObjPtr;
			return;
		}
		break;

	case '4': 
		/// Model Load Hedra
		newObjPtr = new Object();
		strcpy(newObjPtr->name,"Hedra");
		if ( LoadModel_vtx( "Hedra", newObjPtr ) == 0 )
		{
			delete newObjPtr;
			return;
		}
		break;

	case '5': 
		/// Model Load Teapot
		newObjPtr = new Object();
		strcpy(newObjPtr->name,"teapot");
		if ( LoadModel_vtx( "teapot", newObjPtr ) == 0 )
		{
			delete newObjPtr;
			return;
		}
		break;
	
	case '6': 
		/// Model Load Torous
		newObjPtr = new Object();
		strcpy(newObjPtr->name,"torous");
		if ( LoadModel_vtx( "torous", newObjPtr ) == 0 )
		{
			delete newObjPtr;
			return;
		}
		break;

	case '7': 
		/// Model Load Torous_knot
		newObjPtr = new Object();
		strcpy(newObjPtr->name,"Torous_knot");
		if ( LoadModel_vtx( "Torous_knot", newObjPtr ) == 0 )
		{
			delete newObjPtr;
			return;
		}
		break;

	case '8': 
		/// Model Load Male
		newObjPtr = new Object();
		strcpy(newObjPtr->name,"Male");
		if ( LoadModel_vtx( "male", newObjPtr ) == 0 )
		{
			delete newObjPtr;
			return;
		}
		break;

	case '9': 
		/// Model Load cube
		newObjPtr = new Object();
		strcpy(newObjPtr->name,"cube");
		if ( LoadModel_vtx( "cuboid", newObjPtr ) == 0 )
		{
			delete newObjPtr;
			return;
		}
		break;
	
	case 'G':	/// Grid
		ptr = startObject;
		/// check for existing GRID
		while(ptr)
		{
			if(strcmp(ptr->name,"GRID") == 0) return;
			ptr = ptr->nextObject;
		}
		newObjPtr = new Grid();
		strcpy(newObjPtr->name,"GRID");
		break;

	default:
		return;
	}

	if(!startObject)
	{
		startObject = ptr = newObjPtr;
		startObject->nextObject = NULL;
	}
	else
	{
		ptr = startObject;
		while(ptr->nextObject) ptr = ptr->nextObject;
		ptr->nextObject = newObjPtr;
		ptr = ptr->nextObject;
		ptr->nextObject = NULL;
	}
	
	d3ddev->CreateVertexBuffer(ptr->numofVertices * sizeof(CUSTOMVERTEX), 0, CUSTOMFVF, D3DPOOL_MANAGED, &(ptr->buffer), NULL);
	VOID* pVoid;											// the void pointer
	ptr->buffer->Lock(0, 0, (void**)&pVoid, 0);				// lock the vertex buffer
    memcpy(pVoid, ptr->vert, sizeof(CUSTOMVERTEX) * ptr->numofVertices);		// copy the vertices to the locked buffer
	ptr->buffer->Unlock();									// unlock the vertex buffer

	d3ddev->CreateIndexBuffer(ptr->sizeofIndices, 0, D3DFMT_INDEX16 ,D3DPOOL_MANAGED, &(ptr->indexBuffer), NULL);
	ptr->indexBuffer->Lock(0, 0, (void**)&pVoid, 0);		// lock the vertex buffer
	memcpy(pVoid, ptr->indices, ptr->sizeofIndices);		// copy the vertices to the locked buffer
	ptr->indexBuffer->Unlock();								// unlock the vertex buffer

	deSelect();
	currentSelectedObject = ptr;
	Select(currentSelectedObject);

	return;
}

void DrawObject(LPDIRECT3DDEVICE9 d3ddev)
{	
	setCurrentCamera(d3ddev);
	
	Object *ptr;
	ptr = startObject;
	
	// begins the 3D scene
	if(FAILED(d3ddev->BeginScene())) 
		return;	
	while(ptr)
	{
		if(ptr->renderFlag)
		{
			d3ddev->SetFVF(CUSTOMFVF);					// select which vertex format we are using
			d3ddev->SetTexture(0, ptr->texture);		// apply texture if available
			d3ddev->SetStreamSource(0, ptr->buffer, 0, sizeof(CUSTOMVERTEX));	// select the vertex buffer to display
			d3ddev->SetIndices(ptr->indexBuffer);
			d3ddev->DrawIndexedPrimitive((D3DPRIMITIVETYPE)ptr->drawPrimitiveType, 0, 0,ptr->numofVertices, 0,ptr->numofIndices);
		}
		ptr = ptr->nextObject;
	}
	// ends the 3D scene
	d3ddev->EndScene();									
}

void bufferCleanup()
{
	if (!startObject) return;
	Object *ptr, *ptr2;
	ptr = ptr2 = startObject;
	while(ptr)
	{
		ptr = ptr->nextObject;
		if(ptr2) delete ptr2;
		ptr2 = ptr;
	}
	startObject = NULL;
	currentSelectedObject = NULL;
}

void deSelect()
{
	if(currentSelectedObject)
	{
		for(int i=0; i<currentSelectedObject->numofVertices; i++)
			currentSelectedObject->vert[i].color = currentSelectedObject->color;
		UpdateVertexInfo( currentSelectedObject, NULL);
	}
}

void Select(Object *obj)
{
	if(obj && strcmp(obj->name, "GRID") != 0)
	{
		for(int i=0; i<obj->numofVertices; i++)
			obj->vert[i].color = D3DCOLOR_XRGB (0xff, 0xff, 0xff);;
		UpdateVertexInfo( obj, NULL);
	}
}

void selectNextObject()
{
	if(currentSelectedObject)
	{
		deSelect();

		if(currentSelectedObject->nextObject == NULL && startObject->nextObject)
			currentSelectedObject = startObject->nextObject;
		else currentSelectedObject = currentSelectedObject->nextObject;
		
		Select(currentSelectedObject);
	}
}

void rotateObject(Object *obj, float angleX, float angleY, float angleZ)
{
	if(obj == NULL) 
		obj = currentSelectedObject;
	if (obj == NULL || strcmp(obj->name, "GRID") == 0) return;

	D3DXMATRIX rotate, result;
	D3DXMATRIX backTranslate, forwardTranslate;

	D3DXMatrixTranslation(&backTranslate, -obj->PIVOT.x, -obj->PIVOT.y, -obj->PIVOT.z );
	D3DXMatrixTranslation(&forwardTranslate, obj->PIVOT.x, obj->PIVOT.y, obj->PIVOT.z );

	D3DXMatrixRotationYawPitchRoll( &rotate, D3DXToRadian(angleY) , D3DXToRadian(angleX), D3DXToRadian(angleZ) );

	result = backTranslate * rotate * forwardTranslate;
	UpdateVertexInfo( obj, &result);
}

void translateObject(Object *obj, float x, float y, float z)
{
	if(obj == NULL) 
		obj = currentSelectedObject;
	if (obj == NULL || strcmp(obj->name, "GRID") == 0) return;

	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, x, y, z);
	
	obj->PIVOT.x += x;
	obj->PIVOT.y += y;
	obj->PIVOT.z += z;

	UpdateVertexInfo( obj, &translate);
}

void translatePivot(Object *obj, float x, float y, float z)
{
	if(obj == NULL) 
		obj = currentSelectedObject;
	if (obj == NULL || strcmp(obj->name, "GRID") == 0) return;

	obj->PIVOT.x += x;
	obj->PIVOT.y += y;
	obj->PIVOT.z += z;
}

void scaleObject(Object *obj, float sx, float sy, float sz)
{
	if(obj == NULL) 
		obj = currentSelectedObject;
	if (obj == NULL || strcmp(obj->name, "GRID") == 0) return;

	D3DXMATRIX scale, result;
	D3DXMATRIX backTranslate, forwardTranslate;

	D3DXMatrixTranslation(&backTranslate, -obj->PIVOT.x, -obj->PIVOT.y, -obj->PIVOT.z );
	D3DXMatrixTranslation(&forwardTranslate, obj->PIVOT.x, obj->PIVOT.y, obj->PIVOT.z );

	D3DXMatrixScaling( &scale, sx, sy, sz);
	result = backTranslate * scale * forwardTranslate;
	
	UpdateVertexInfo( obj, &result);
}

void UpdateVertexInfo( Object *obj, D3DXMATRIX *mat)
{
	if(obj == NULL)
		obj = currentSelectedObject;
	if(obj == NULL)
		return;

	if(mat)
		for(int i=0; i<obj->numofVertices; i++)
		{
			float xx, yy, zz;
			xx = obj->vert[i].x * mat->_11 + obj->vert[i].y * mat->_21 + obj->vert[i].z * mat->_31 + mat->_41;
			yy = obj->vert[i].x * mat->_12 + obj->vert[i].y * mat->_22 + obj->vert[i].z * mat->_32 + mat->_42; 
			zz = obj->vert[i].x * mat->_13 + obj->vert[i].y * mat->_23 + obj->vert[i].z * mat->_33 + mat->_43;

			obj->vert[i].x = xx;
			obj->vert[i].y = yy;
			obj->vert[i].z = zz;
		}
	
	VOID* pVoid;											// the void pointer
	obj->buffer->Lock(0, 0, (void**)&pVoid, 0);				// lock the vertex buffer
    memcpy(pVoid, obj->vert, sizeof(CUSTOMVERTEX) * obj->numofVertices);		// copy the vertices to the locked buffer
	obj->buffer->Unlock();
}

void updateAllVertexBuffer(LPDIRECT3DDEVICE9 d3ddev)
{
	Object *ptr;
	ptr = startObject;
	
	while(ptr)
	{
		if(ptr->buffer)
			ptr->buffer->Release();

		d3ddev->CreateVertexBuffer(ptr->numofVertices * sizeof(CUSTOMVERTEX), 0, CUSTOMFVF, D3DPOOL_MANAGED, &(ptr->buffer), NULL);
		VOID* pVoid;											// the void pointer
		ptr->buffer->Lock(0, 0, (void**)&pVoid, 0);				// lock the vertex buffer
		memcpy(pVoid, ptr->vert, sizeof(CUSTOMVERTEX) * ptr->numofVertices);		// copy the vertices to the locked buffer
		ptr->buffer->Unlock();									// unlock the vertex buffer
		
		ptr = ptr->nextObject;
	}
}

void updateAllIndexBuffer(LPDIRECT3DDEVICE9 d3ddev)
{
	Object *ptr;
	ptr = startObject;
	
	while(ptr)
	{
		if(ptr->indexBuffer)
			ptr->indexBuffer->Release();

		d3ddev->CreateIndexBuffer(ptr->sizeofIndices, 0, D3DFMT_INDEX16 ,D3DPOOL_MANAGED, &(ptr->indexBuffer), NULL);

		VOID* pVoid;											// the void pointer
		ptr->indexBuffer->Lock(0, 0, (void**)&pVoid, 0);		// lock the vertex buffer
		memcpy(pVoid, ptr->indices, ptr->sizeofIndices);		// copy the vertices to the locked buffer
		ptr->indexBuffer->Unlock();								// unlock the vertex buffer

		ptr = ptr->nextObject;
	}
}

void selectObject(LPDIRECT3DDEVICE9 d3ddev, int x, int y)
{
	Object *ptr;
	if(!startObject->nextObject) return;
	ptr = startObject->nextObject;

	D3DXVECTOR3 Near, Dir;
	computeRay3D(d3ddev, x, y, &Near, &Dir);

	D3DXVECTOR3 lowerVertex, higherVertex;
	while(ptr)
	{
		computeBoundingBox(ptr, &lowerVertex, &higherVertex);
		if( D3DXBoxBoundProbe(&lowerVertex, &higherVertex, &Near, &Dir) == TRUE)
		{
			deSelect();
			Select(ptr);
			currentSelectedObject = ptr;
			return;
		}
		ptr = ptr->nextObject;
	}	
	deSelect();
	currentSelectedObject = NULL;
}

void gridStatus( bool flag)
{
	Object *ptr;
	ptr = startObject;

	while(ptr)
	{
		if(strcmp(ptr->name,"GRID") == 0) 
		{
			ptr->renderFlag = flag;
			return;
		}
		ptr = ptr->nextObject;
	}
}

void getObjectPivot(D3DXVECTOR3 *vec)
{
	vec->x = currentSelectedObject->PIVOT.x;
	vec->y = currentSelectedObject->PIVOT.y;
	vec->z = currentSelectedObject->PIVOT.z;
}

bool isObjectSelected()
{
	if(currentSelectedObject)
		return TRUE;
	return FALSE;
}

void deleteSelectObject()
{
	Object *ptr;
	ptr = startObject;
	if( !startObject->nextObject ) 
		return;
	while(ptr->nextObject != currentSelectedObject)
		ptr = ptr->nextObject;
	ptr->nextObject = currentSelectedObject->nextObject;

	delete (currentSelectedObject);
	currentSelectedObject = NULL;
}