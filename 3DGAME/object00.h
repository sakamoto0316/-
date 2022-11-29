#ifndef _OBLECT00_H_
#define _OBLECT00_H_

//モデルの構造体
typedef struct
{
	D3DXVECTOR3 pos;							//位置
	D3DXVECTOR3 posOld;							//位置
	D3DXVECTOR3 move;							//位置
	D3DXVECTOR3 rot;							//向き
	D3DXMATRIX mtxWorld;						//ワールドマトリックス
	bool bUse;						//モデルの最大
}Object00;

//プロトタイプ宣言
void InitObject00(void);
void UninitObject00(void);
void UpdateObject00(void);
void DrawObject00(void);
void SetObject00(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot);
Object00 * GetObject00(void);
#endif