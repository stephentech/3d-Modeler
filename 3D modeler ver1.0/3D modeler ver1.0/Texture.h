
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <dataStructure.h>

void calculateTextureCoordinates(Object*);
bool applyTexture(LPDIRECT3DDEVICE9, LPCWSTR);
void deleteTexture();

#endif