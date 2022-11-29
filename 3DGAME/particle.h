#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, float nSize, int nType);
#endif
