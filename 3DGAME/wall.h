#ifndef _WALL_H_
#define _WALL_H_

#define MAX_WALL (1)	//影の最大数
#define MAX_WIDTH (50.0f)	//影の横幅
#define MAX_HEIGHT (100.0f)	//影の縦幅

//壁構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;							//位置
	D3DXVECTOR3 rot;							//向き
	D3DXMATRIX mtxWorld;						//ワールドマトリックス
}Wall;

//プロトタイプ宣言
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 min, D3DXVECTOR3 max);
void CollisionBillboard(Wall*pWall);
Wall * GetWall(void);
#endif