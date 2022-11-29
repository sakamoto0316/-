#include "main.h"
#include "explosion.h"

//マクロ定義
#define MAX_EXPLOSION (128)		//爆発の最大数
#define SIZE_EXPLOSION (10.0f)	//爆発の大きさ

//爆発構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//現在位置
	D3DXVECTOR3 posOld;				//１フレーム前の位置
	D3DXMATRIX mtxWorld;			//ワールドマトリックス
	D3DXCOLOR col;					//色
	int nCounterAnim;				//アニメーションカウンター
	int nPatternAnim;				//アニメーションパターンNo.
	bool bUse;						//使用しているかどうか
}Explosion;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;			//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;		//頂点バッファのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];					//爆発の情報

//====================================================================
//爆発の初期化処理
//====================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	int nCntExplosion;

	//デバイスの所得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\magic001.png",
		&g_pTextureExplosion);

	//爆発の情報の初期化
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;	//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_3D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点座標の設定 
		pVtx[0].pos = D3DXVECTOR3(-5.0f, +5.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+5.0f, +5.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-5.0f, -5.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+5.0f, -5.0f, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.0f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);
		pVtx[1].tex = D3DXVECTOR2(0.2f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.0f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.2f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);
		pVtx[3].tex = D3DXVECTOR2(0.2f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.2f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);

		pVtx += 4;	//頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//====================================================================
//爆発の終了処理
//====================================================================
void UninitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//====================================================================
//爆発の更新処理
//====================================================================
void UpdateExplosion(void)
{
	int nCntExplosion;

	VERTEX_3D*pVtx;	//頂点ポインタを所得

					//頂点バッファをロックし、両店情報へのポインタを所得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆発が使用されている
			g_aExplosion[nCntExplosion].nCounterAnim++;	//カウンターを加算
			if (g_aExplosion[nCntExplosion].nCounterAnim % 3 == 0)
			{
				//パターンNo.を更新する
				g_aExplosion[nCntExplosion].nPatternAnim++;

				//頂点座標の設定 
				pVtx[0].pos = D3DXVECTOR3(-5.0f, +5.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(+5.0f, +5.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-5.0f, -5.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(+5.0f, -5.0f, 0.0f);

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.0f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.0f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.2f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.2f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);

				if (g_aExplosion[nCntExplosion].nPatternAnim > 25)
				{
					g_aExplosion[nCntExplosion].bUse = false;	//使用していない状態にする
				}

			}
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//====================================================================
//爆発の描画処理
//====================================================================
void DrawExplosion(void)
{
	int nCntEx;

	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;	//ビューマトリックス所得用

						//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (nCntEx = 0; nCntEx < MAX_EXPLOSION; nCntEx++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aExplosion[nCntEx].mtxWorld);

		//ビューマトリックスを所得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//ポリゴンをカメラに対して正面を向ける
		D3DXMatrixInverse(&g_aExplosion[nCntEx].mtxWorld, NULL, &mtxView);	//逆行列を求める
		g_aExplosion[nCntEx].mtxWorld._41 = 0.0f;
		g_aExplosion[nCntEx].mtxWorld._42 = 0.0f;
		g_aExplosion[nCntEx].mtxWorld._43 = 0.0f;

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntEx].pos.x, g_aExplosion[nCntEx].pos.y, g_aExplosion[nCntEx].pos.z);

		D3DXMatrixMultiply(&g_aExplosion[nCntEx].mtxWorld, &g_aExplosion[nCntEx].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntEx].mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureExplosion);

		if (g_aExplosion[nCntEx].bUse == true)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				4 * nCntEx,										//プリミティブ(ポリゴン)数
				2);
		}
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//====================================================================
//爆発の設定処理
//====================================================================
void SetExplosion(D3DXVECTOR3 pos, int nType)
{
	int nCntExplosion;

	for (nCntExplosion = 0; nCntExplosion <MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//爆発が使用されていない
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;	//爆発を使用している状態にする

			break;
		}
	}
}