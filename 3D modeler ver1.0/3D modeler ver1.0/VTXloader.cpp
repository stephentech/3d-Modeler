
#include <VTXloader.h>
#include <stdio.h>

FILE *fp;

bool LoadModel_vtx(char name[20], Object *obj )
{
	char filename[50];
	sprintf(filename,"predefinedObjects\\%s.vtx",name);
	fp = fopen(filename,"r");
	//fp = fopen("sphere.vtx","r");
	if(!fp)
	{
		return 0;
	}
	
	/// get list of vertices...
	obj->numofVertices = getNumInt();
	obj->vert = new CUSTOMVERTEX[obj->numofVertices];
	for( int i=0; i<obj->numofVertices; i++ )
	{
		obj->vert[i].x = getNumFloat() ;
		obj->vert[i].y = getNumFloat() ;
		obj->vert[i].z = getNumFloat() ;

		obj->vert[i].color = obj->color;
		obj->vert[i].u = 0.0f;		
		obj->vert[i].v = 0.0f;		

		getNumFloat();		/// move to next line...
	}

	// shift the File-pointer one ahead
	fgetc(fp);

	// get list of indices...
	obj->numofIndices = getNumInt();
	obj->sizeofIndices = obj->numofIndices * sizeof(INDICE);
	obj->indices = new INDICE[obj->numofIndices];
	for( int i=0; i<obj->numofIndices; i++ )
	{
		getNumInt();
		obj->indices[i].v1 = getNumInt();
		obj->indices[i].v2 = getNumInt();
		obj->indices[i].v3 = getNumInt();
		getNumInt();
	}

	fclose(fp);
	return 1;
}

float getNumFloat()
{
	float num = 0.0f, negetiveFlag = 1.0f;
	int floatFlag = 0, counter = 0, count = 1;
	char ch;
	while(counter < 20)
	{
		counter++;
		ch = fgetc(fp);
		switch(ch)
		{
		case '1':		case '2':		case '3':		case '4':
		case '5':		case '6':		case '7':		case '8':
		case '9':		case '0':
			if(floatFlag)
			{
				float n =1.0f;
				for(int i=0; i<count; i++)
					n = n * 0.1;
				num = num + (ch - '0') * n;
				count++;
			}
			else
			{
				num = num*10 + ch - '0';
			}
			break;

		case '.':
			floatFlag = 1;
			break;

		case ',':		case ';':
			return(negetiveFlag * num);
			break;

		case '-':
			negetiveFlag = -1.0f;
		}
	}
	return 0;
}

int getNumInt()
{
	int num = 0, counter = 0;
	char ch;
	while(counter < 20)
	{
		counter++;
		ch = fgetc(fp);
		switch(ch)
		{
		case '1':		case '2':		case '3':		case '4':
		case '5':		case '6':		case '7':		case '8':
		case '9':		case '0':
			num = num*10 + (ch - '0');
			break;
	
		case ',':		case ';':
			return(num);
			break;
		}
	}
	return 0;
}
