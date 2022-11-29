#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//ブロックの種類
typedef enum
{
	CAMERA_MODE_1 = 0,		//一人称視点
	CAMERA_MODE_3,			//三人称視点
	CAMERA_MODE_MAX
}CAMERA_MODE;

//カメラの構造体を定義
typedef struct
{
	D3DXVECTOR3 posV;			//視点
	D3DXVECTOR3 posR;			//注視点
	D3DXVECTOR3 vecU;			//上方向ベクトル
	D3DXMATRIX mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX mtxView;			//ビューマトリックス
	D3DXVECTOR3 rot;
	D3DXVECTOR3 rotOld;
	D3DXVECTOR3 posVDest;		//目的の視点
	D3DXVECTOR3 posRDest;		//目的の注視点
	CAMERA_MODE Mode;
}Camera;

//プロトタイプ宣言
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
D3DXVECTOR3 GetposV(void);
D3DXVECTOR3 GetposR(void);
D3DXVECTOR3 GetvecU(void);
D3DXVECTOR3 Getrot(void);
CAMERA_MODE GetCameraMode(void);
#endif