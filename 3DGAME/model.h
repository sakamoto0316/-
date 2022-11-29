#ifndef _MODEL_H_
#define _MODEL_H_

//モデルの構造体
typedef struct
{
	D3DXVECTOR3 pos;							//現在の位置
	D3DXVECTOR3 posOld;							//過去の位置
	D3DXVECTOR3 move;							//移動量
	D3DXVECTOR3 vecMove;						//移動ベクトル
	D3DXVECTOR3 rot;							//向き
	D3DXMATRIX mtxWorld;						//ワールドマトリックス
	D3DXVECTOR3 g_vtxMin;						//モデルの最小
	D3DXVECTOR3 g_vtxMax;						//モデルの最大
}Model;

//プロトタイプ宣言
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
Model * GetModel(void);
#endif