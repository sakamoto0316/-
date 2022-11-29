#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#define MAX_BILLBOARD (128)		//影の最大数

//ビルボード構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;							//位置
	D3DXVECTOR3 posOld;							//位置
	D3DXMATRIX mtxWorld;						//ワールドマトリックス
	float rot;									//向き
	int nLife;									//寿命
	int nIndex;
	bool bUse;									//使用しているかどうか
}Billboard;

//プロトタイプ宣言
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
void SetBillboard(D3DXVECTOR3 pos, float rot);
Billboard * GetBillboard(void);
#endif