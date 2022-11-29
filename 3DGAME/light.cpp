#include "main.h"
#include "light.h"

#define MAX_LIGHT (3)	//使用するライトの数

//グローバル変数
D3DLIGHT9 g_light[MAX_LIGHT];	//ライトの情報

//====================================================================
//ポリゴンの初期化処理
//====================================================================
void InitLight(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir[MAX_LIGHT];	//設定用方向ベクトル

									//ライトの情報をクリアする
	ZeroMemory(&g_light, sizeof(D3DLIGHT9));

	for (int nCntlight = 0; nCntlight < MAX_LIGHT; nCntlight++)
	{
		//ライトの種類を設定
		g_light[nCntlight].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		g_light[nCntlight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//ライトの方向を設定
		switch (nCntlight)
		{
		case 0:
			vecDir[nCntlight] = D3DXVECTOR3(0.2f, -0.8f, -0.5f);
			break;
		case 1:
			vecDir[nCntlight] = D3DXVECTOR3(-0.5f, -0.8f, -0.5f);
			break;
		case 2:
			vecDir[nCntlight] = D3DXVECTOR3(0.0f, -0.8f, 0.6f);
			break;
		}
		D3DXVec3Normalize(&vecDir[nCntlight], &vecDir[nCntlight]);	//ベクトルを正規化する
		g_light[nCntlight].Direction = vecDir[nCntlight];

		//ライトを設定する
		pDevice->SetLight(nCntlight, &g_light[nCntlight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntlight, TRUE);
	}
}

//====================================================================
//ポリゴンの終了処理
//====================================================================
void UninitLight(void)
{

}

//====================================================================
//ポリゴンの更新処理
//====================================================================
void UpdateLight(void)
{

}