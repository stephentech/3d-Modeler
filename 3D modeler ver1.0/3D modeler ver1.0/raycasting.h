
#ifndef _RAYCASTING_H_
#define _RAYCASTING_H_

#include <dataStructure.h>

void computeRay3D(LPDIRECT3DDEVICE9, int, int, D3DXVECTOR3*, D3DXVECTOR3*);
bool computeBoundingBox(Object*, D3DXVECTOR3*, D3DXVECTOR3*);

#endif