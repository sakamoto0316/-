#include "main.h"
#include "effect.h"

//マクロ定義
#define MAX_EFFECT (4096)		//エフェクトの最大数
#define EFFECT_SIZE (6.0f)		//エフェクトの大きさ

//エフェクト構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXMATRIX mtxWorld;						//ワールドマトリックス
	D3DXCOLOR col;		//色
	float fRadius;		//半径(大きさ)
	int nLifeMax;		//寿命の最大値
	int nLife;			//寿命
	bool bUse;			//使用しているかどうか
}Effect;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;				//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;		//頂点バッファのポインタ
Effect g_aEffect[MAX_EFFECT];							//エフェクトの情報
int g_EffectNum;

//====================================================================
//エフェクトの初期化処理
//====================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	int nCntEffect;

	//デバイスの所得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&g_pTextureEffect);

	//エフェクトの情報の初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].fRadius = 0;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].nLifeMax = g_aEffect[nCntEffect].nLife;
		g_aEffect[nCntEffect].bUse = false;	//使用していない状態にする
	}
	g_EffectNum = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_3D*pVtx;	//頂点ポインタを所得

					//頂点バッファをロックし、両店情報へのポインタを所得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定 
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//====================================================================
//エフェクトの終了処理
//====================================================================
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//====================================================================
//エフェクトの更新処理
//====================================================================
void UpdateEffect(void)
{
	int nCntEffect;

	VERTEX_3D*pVtx;	//頂点ポインタを所得

					//頂点バッファをロックし、両店情報へのポインタを所得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている

			g_aEffect[nCntEffect].nLife--;
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;	//使用していない状態にする
				g_EffectNum--;
			}

		 //頂点座標の設定 
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius * (float)g_aEffect[nCntEffect].nLife / (float)g_aEffect[nCntEffect].nLifeMax, +g_aEffect[nCntEffect].fRadius * (float)g_aEffect[nCntEffect].nLife / (float)g_aEffect[nCntEffect].nLifeMax, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius * (float)g_aEffect[nCntEffect].nLife / (float)g_aEffect[nCntEffect].nLifeMax, +g_aEffect[nCntEffect].fRadius * (float)g_aEffect[nCntEffect].nLife / (float)g_aEffect[nCntEffect].nLifeMax, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius * (float)g_aEffect[nCntEffect].nLife / (float)g_aEffect[nCntEffect].nLifeMax, -g_aEffect[nCntEffect].fRadius * (float)g_aEffect[nCntEffect].nLife / (float)g_aEffect[nCntEffect].nLifeMax, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius * (float)g_aEffect[nCntEffect].nLife / (float)g_aEffect[nCntEffect].nLifeMax, -g_aEffect[nCntEffect].fRadius * (float)g_aEffect[nCntEffect].nLife / (float)g_aEffect[nCntEffect].nLifeMax, 0.0f);
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//====================================================================
//エフェクトの描画処理
//====================================================================
void DrawEffect(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;	//ビューマトリックス所得用
	int nCntEffect;

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

		//ビューマトリックスを所得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//ポリゴンをカメラに対して正面を向ける
		D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorld, NULL, &mtxView);	//逆行列を求める
		g_aEffect[nCntEffect].mtxWorld._41 = 0.0f;
		g_aEffect[nCntEffect].mtxWorld._42 = 0.0f;
		g_aEffect[nCntEffect].mtxWorld._43 = 0.0f;

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);

		D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureEffect);

		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている
		 //ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntEffect * 4,							//プリミティブ(ポリゴン)数
				2);
		}
	}

	////αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//====================================================================
//エフェクトの設定処理
//====================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife)
{
	int nCntEffect;

	for (nCntEffect = 0; nCntEffect <MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//エフェクトが使用されていない
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].bUse = true;	//エフェクトを使用している状態にする
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].nLifeMax = nLife;
			g_EffectNum++;
			break;
		}
	}
}

int GetEffectNum(void)
{
	return g_EffectNum;
}