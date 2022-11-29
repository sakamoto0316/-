#include "main.h"
#include "wall.h"
#include "billboard.h"
#include "shadow.h"
#include "explosion.h"
#include "particle.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureWall;				//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall;			//頂点バッファのポインタ
Wall g_Wall[MAX_WALL];							//壁の情報
int g_nIndexWall[MAX_WALL];
//====================================================================
//壁の初期化処理
//====================================================================
void InitWall(void)
{
	int nCntWall;

	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ENEMY00.png",
		&g_pTextureWall);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		switch (nCntWall)
		{
		case 0:
			g_Wall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.25f, 0.0f);
			break;

		case 1:
			g_Wall[nCntWall].pos = D3DXVECTOR3(50.0f, 0.0f, 200.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			break;

		case 2:
			g_Wall[nCntWall].pos = D3DXVECTOR3(-50.0f, 0.0f, -200.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
			break;

		case 3:
			g_Wall[nCntWall].pos = D3DXVECTOR3(50.0f, 0.0f, -200.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
			break;

		case 4:
			g_Wall[nCntWall].pos = D3DXVECTOR3(200.0f, 0.0f, -50.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
			break;

		case 5:
			g_Wall[nCntWall].pos = D3DXVECTOR3(200.0f, 0.0f, 50.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
			break;

		case 6:
			g_Wall[nCntWall].pos = D3DXVECTOR3(-200.0f, 0.0f, -50.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
			break;

		case 7:
			g_Wall[nCntWall].pos = D3DXVECTOR3(-200.0f, 0.0f, 50.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
			break;
		}
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//頂点座標の設定 
		pVtx[0].pos = D3DXVECTOR3(-MAX_WIDTH, MAX_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+MAX_WIDTH, MAX_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-MAX_WIDTH, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+MAX_WIDTH, 0.0f, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

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
	g_pVtxBuffWall->Unlock();

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//影の設定
		g_nIndexWall[nCntWall] = SetShadow();

		//影の位置を設定
		SetPositionShadow(g_nIndexWall[nCntWall], g_Wall[nCntWall].pos);
	}
}

//====================================================================
//壁の終了処理
//====================================================================
void UninitWall(void)
{
	//テクスチャの破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//====================================================================
//壁の更新処理
//====================================================================
void UpdateWall(void)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		CollisionBillboard(&g_Wall[nCntWall]);
	}
	//g_Wall[0].rot.y += 0.001f;
}

//====================================================================
//壁の描画処理
//====================================================================
void DrawWall(void)
{
	int nCntWall;

	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Wall[nCntWall].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);

		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);

		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCntWall].mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureWall);

		//壁の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			4 * nCntWall,										//プリミティブ(ポリゴン)数
			2);
	}
}

//====================================================================
//壁との当たり判定処理
//====================================================================
void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{

		//if (g_Wall[nCntWall].rot.y == 0.0f || g_Wall[nCntWall].rot.y == D3DX_PI)
		//{//Z軸に対して正面の壁
		//	if (
		//		(pPos->z + max.z >= g_Wall[nCntWall].pos.z && pPosOld->z + max.z< g_Wall[nCntWall].pos.z ||
		//		pPos->z + min.z <= g_Wall[nCntWall].pos.z && pPosOld->z + min.z > g_Wall[nCntWall].pos.z) &&
		//		pPos->x + max.x >= g_Wall[nCntWall].pos.x - MAX_WIDTH &&
		//		pPos->x + min.x <= g_Wall[nCntWall].pos.x + MAX_WIDTH &&
		//		pPos->y + max.y >= g_Wall[nCntWall].pos.y &&
		//		pPos->y + min.y <= g_Wall[nCntWall].pos.y + MAX_HEIGHT
		//		)
		//	{//壁とプレイヤーが当たった
		//		pPos->z = pPosOld->z;
		//	}
		//}

		//if (g_Wall[nCntWall].rot.y == D3DX_PI * 0.5f || g_Wall[nCntWall].rot.y == D3DX_PI * -0.5f)
		//{//X軸に対して正面の壁
		//	if (
		//		(pPos->x + max.x >= g_Wall[nCntWall].pos.x && pPosOld->x + max.x < g_Wall[nCntWall].pos.x ||
		//		pPos->x + min.x <= g_Wall[nCntWall].pos.x && pPosOld->x + min.x > g_Wall[nCntWall].pos.x) &&
		//		pPos->z + max.z >= g_Wall[nCntWall].pos.z - MAX_WIDTH &&
		//		pPos->z + min.z <= g_Wall[nCntWall].pos.z + MAX_WIDTH &&
		//		pPos->y + max.y >= g_Wall[nCntWall].pos.y &&
		//		pPos->y + min.y <= g_Wall[nCntWall].pos.y + MAX_HEIGHT
		//		)
		//	{//壁とプレイヤーが当たった
		//		pPos->x = pPosOld->x;
		//	}
		//}
	}
}

//====================================================================
//弾との当たり判定処理
//====================================================================
void CollisionBillboard(Wall*pWall)
{
	Billboard * pBillboard;		//弾の情報へのポインタ

	D3DXVECTOR3 vecToPos0;
	D3DXVECTOR3 vecToPos1;
	D3DXVECTOR3 vecToPos3;
	D3DXVECTOR3 vecToPos2;

	D3DXVECTOR3 vecToPosOld0;
	D3DXVECTOR3 vecToPosOld1;
	D3DXVECTOR3 vecToPosOld3;
	D3DXVECTOR3 vecToPosOld2;

	D3DXVECTOR3 vecLine01;
	D3DXVECTOR3 vecLine13;
	D3DXVECTOR3 vecLine32;
	D3DXVECTOR3 vecLine20;

	//敵の所得
	pBillboard = GetBillboard();		//敵の情報の先頭アドレスが代入される

	VERTEX_3D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pBillboard++)
	{
		if (pBillboard->bUse == true)
		{
			vecToPos0.x = (sinf(pWall->rot.y) * (pWall->pos.x + pVtx[0].pos.x));
			vecToPos0.y = 0.0f;
			vecToPos0.z = pBillboard->pos.z - (cosf(pWall->rot.y) * (pWall->pos.z + pVtx[0].pos.z));

			//vecToPosOld0 = pBillboard->posOld - (pWall->pos + pVtx[0].pos);

			//100
			vecLine01.x = cosf(pWall->rot.y) * (pVtx[1].pos.x - pVtx[0].pos.x);
			vecLine01.y = 0.0f;
			vecLine01.z = sinf(pWall->rot.y) * (pVtx[1].pos.x - pVtx[0].pos.x);


			vecToPos3 = pBillboard->pos - (pWall->pos + pVtx[3].pos);
			vecToPosOld3 = pBillboard->posOld - (pWall->pos + pVtx[3].pos);
			vecLine32 = pWall->pos + pVtx[2].pos - (pWall->pos + pVtx[3].pos);

			if (((vecLine01.z * vecToPos0.x) - (vecLine01.x * vecToPos0.z)) < 0 /*&& ((vecLine01.z * vecToPosOld0.x) - (vecLine01.x * vecToPosOld0.z)) > 0 ||
				((vecLine32.z * vecToPos3.x) - (vecLine32.x * vecToPos3.z)) > 0 && ((vecLine32.z * vecToPosOld3.x) - (vecLine32.x * vecToPosOld3.z)) < 0*/)
			{
					pBillboard->bUse = false;
					falseShadow(pBillboard->nIndex);
					SetExplosion(pBillboard->pos, 0);
					SetParticle(pBillboard->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 60, 5.0f, 4);
			}
		}
		//pVtx += 4;	//頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}
//====================================================================
//壁の所得
//====================================================================
Wall * GetWall(void)
{
	return &g_Wall[0];
}