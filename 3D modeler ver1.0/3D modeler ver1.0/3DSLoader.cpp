
#include <3DSLoader.h>
#include <dataStructure.h>
#include <stdio.h>

extern Object *startObject;

int Load3DS(char filename[])
{
	/// traverse to end of object List
	Object *ptr;
	ptr = startObject;
	while(ptr->nextObject)
	{
		ptr = ptr->nextObject;
	}

	FILE *file;
	file = fopen(filename, "rb");
	if(!file) 
	{
		MessageBox(NULL,L"could not open file", L"Error",MB_OK);
		return 0;
	}
		
	unsigned short chunkID = 0;
	int len = 0;
	
	fread( &chunkID, 2, 1, file );
	fread( &len, 4, 1, file );

	while(1)
	{
		if( fread( &chunkID, 2, 1, file ) == 0 )
			break;
		fread( &len, 4, 1, file );

		switch(chunkID)
		{
			case 0x3d3d:
				break;
		
			case 0x4100:
			break;

			case 0x4000:
			{
				/// create new obj
				ptr->nextObject = new Object();
				ptr = ptr->nextObject;
				ptr->nextObject = NULL;
				//MessageBox(NULL,L"NewObject", L"Info",MB_OK);

				/// extract name of object
				int i = 0;
				char ch, buffer[20];
				do
				{
					fread( &ch, 1, 1, file );
					buffer[i] = ch;
					i++;
				}while(i < 20 && ch != '\0' && ch != '\n' );

				strcpy(ptr->name, buffer);
			}
			break;

			case 0x4110: 
			{
				int vCnt;
				fread( &vCnt, 2, 1, file );
				ptr->numofVertices = vCnt;

				ptr->vert = new CUSTOMVERTEX[ptr->numofVertices];
				for(int i=0; i<ptr->numofVertices; i++)
				{
					fread( &(ptr->vert[i].x), 4, 1, file );
					fread( &(ptr->vert[i].y), 4, 1, file );
					fread( &(ptr->vert[i].z), 4, 1, file );
					
					ptr->vert[i].color = ptr->color;
					ptr->vert[i].u = 0.0f;		
					ptr->vert[i].v = 0.0f;		
				}
			}
			break;
	
			case 0x4120:
			{
				int numPolygons;
				fread( &numPolygons, 2, 1, file );
				ptr->numofIndices = numPolygons;

				ptr->indices = new INDICE[ptr->numofIndices];
				for(int i=0; i<ptr->numofIndices; i++)
				{
					fread( &(ptr->indices[i].v1), 4, 1, file );
					fread( &(ptr->indices[i].v2), 4, 1, file );
					fread( &(ptr->indices[i].v3), 4, 1, file );
				}
				ptr->sizeofIndices = ptr->numofIndices * sizeof(INDICE);
			}
			break;

			default:
				fseek( file, len-6, SEEK_CUR );
			break;
		}

	}

	if(file) 	fclose(file);
	return 1;
}