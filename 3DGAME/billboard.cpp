#include "main.h"
#include "billboard.h"
#include "shadow.h"
#include "effect.h"
#include "explosion.h"
#include "particle.h"
#include "wall.h"

#define BILLBOARD_LIFE (200)	//影の寿命
#define BILLBOARD_SIZE (5.0f)	//影の大きさ

//プロトタイプ宣言
void CollisionWall(Billboard*pBillboard, int nCnt);

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBillboard;				//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard;		//頂点バッファのポインタ
Billboard g_aBillboard[MAX_BILLBOARD];

//====================================================================
//ビルボードの初期化処理
//====================================================================
void InitBillboard(void)
{
	int nCntBill;
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BOSS00.png",
		&g_pTextureBillboard);

	for (nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		g_aBillboard[nCntBill].pos = D3DXVECTOR3(90.0f, 10.0f, 50.0f);
		g_aBillboard[nCntBill].nIndex = -1;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		//頂点座標の設定 
		pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE, +BILLBOARD_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+BILLBOARD_SIZE, +BILLBOARD_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f);

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
	g_pVtxBuffBillboard->Unlock();
}

//====================================================================
//ビルボードの終了処理
//====================================================================
void UninitBillboard(void)
{
	//テクスチャの破棄
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//====================================================================
//ビルボードの更新処理
//====================================================================
void UpdateBillboard(void)
{
	int nCntBill;

	for (nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == true)
		{
			g_aBillboard[nCntBill].posOld = g_aBillboard[nCntBill].pos;

			//影の位置を設定
			SetPositionShadow(g_aBillboard[nCntBill].nIndex, g_aBillboard[nCntBill].pos);

			//弾の発射向きを指定する
			g_aBillboard[nCntBill].pos.z += cosf(g_aBillboard[nCntBill].rot);
			g_aBillboard[nCntBill].pos.x += sinf(g_aBillboard[nCntBill].rot);
			g_aBillboard[nCntBill].nLife--;

			//壁との当たり判定
			CollisionWall(&g_aBillboard[nCntBill], nCntBill);
			Wall *pWall = GetWall();


			//弾発射時にエフェクトを呼び出す
			SetEffect(g_aBillboard[nCntBill].pos,D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),8.0f,30);


			//寿命がなくなった時弾を消す
			if (g_aBillboard[nCntBill].nLife <= 0)
			{
				g_aBillboard[nCntBill].bUse = false;
				falseShadow(g_aBillboard[nCntBill].nIndex);
				SetExplosion(g_aBillboard[nCntBill].pos, 0);
			}
		}
	}

	VERTEX_3D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == true)
		{
			//頂点座標の設定 
			pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE, +BILLBOARD_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+BILLBOARD_SIZE, +BILLBOARD_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f);
		}

		pVtx += 4;	//頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//====================================================================
//ビルボードの描画処理
//====================================================================
void DrawBillboard(void)
{
	int nCntBill;

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

	for (nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aBillboard[nCntBill].mtxWorld);

		//ビューマトリックスを所得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//ポリゴンをカメラに対して正面を向ける
		D3DXMatrixInverse(&g_aBillboard[nCntBill].mtxWorld, NULL, &mtxView);	//逆行列を求める
		g_aBillboard[nCntBill].mtxWorld._41 = 0.0f;
		g_aBillboard[nCntBill].mtxWorld._42 = 0.0f;
		g_aBillboard[nCntBill].mtxWorld._43 = 0.0f;

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBill].pos.x, g_aBillboard[nCntBill].pos.y, g_aBillboard[nCntBill].pos.z);

		D3DXMatrixMultiply(&g_aBillboard[nCntBill].mtxWorld, &g_aBillboard[nCntBill].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBill].mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBillboard);

		if (g_aBillboard[nCntBill].bUse == true)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				4 * nCntBill,										//プリミティブ(ポリゴン)数
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
//ビルボードの設定処理
//====================================================================
void SetBillboard(D3DXVECTOR3 pos, float rot)
{
	int nCntBill;

	VERTEX_2D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == false)
		{
			g_aBillboard[nCntBill].pos = pos;
			g_aBillboard[nCntBill].rot = rot;
			g_aBillboard[nCntBill].nLife = BILLBOARD_LIFE;
			g_aBillboard[nCntBill].bUse = true;

			//影の設定
			g_aBillboard[nCntBill].nIndex = SetShadow();
			break;
		}
		pVtx += 4;	//頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//====================================================================
//弾と壁の当たり判定
//====================================================================
void CollisionWall(Billboard*pBillboard,int nCnt)
{
	//Wall * pWall;		//敵の情報へのポインタ
	//int nCntWall;

	//D3DXVECTOR3 vecToPos;
	//D3DXVECTOR3 vecLine;

	////敵の所得
	//pWall = GetWall();		//敵の情報の先頭アドレスが代入される

	//VERTEX_3D*pVtx;	//頂点ポインタを所得

	////頂点バッファをロックし、両店情報へのポインタを所得
	//g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	//{
	//	vecToPos = pBillboard->pos - D3DXVECTOR3(pWall->pos.x - 50, pWall->pos.y, pWall->pos.z);
	//	vecLine = D3DXVECTOR3(pWall->pos.x - 50, pWall->pos.y, pWall->pos.z) - D3DXVECTOR3(pWall->pos.x + 50, pWall->pos.y, pWall->pos.z);

	//	if (((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z)) > 0)
	//	{
	//		pBillboard->bUse = false;
	//		falseShadow(pBillboard->nIndex);
	//		SetExplosion(pBillboard->pos, 0);
	//		SetParticle(pBillboard->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 60, 5.0f, 4);
	//	}

	//	pVtx += 4;	//頂点データのポインタを４つ分進める
	//}

	////頂点バッファをアンロックする
	//g_pVtxBuffBillboard->Unlock();

	//for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	//{
	//	if (pWall->rot.y == 0.0f || pWall->rot.y == D3DX_PI)
	//	{
	//		if (
	//			(pBillboard->pos.z >= pWall->pos.z && pBillboard->posOld.z < pWall->pos.z ||
	//			pBillboard->pos.z <= pWall->pos.z && pBillboard->posOld.z > pWall->pos.z) &&
	//			pBillboard->pos.x >= pWall->pos.x - MAX_WIDTH &&
	//			pBillboard->pos.x <= pWall->pos.x + MAX_WIDTH &&
	//			pBillboard->pos.y >= pWall->pos.y &&
	//			pBillboard->pos.y <= pWall->pos.y + MAX_HEIGHT
	//			)
	//		{//敵と攻撃が当たった
	//			pBillboard->bUse = false;
	//			falseShadow(g_nIndexBillboard[nCnt]);
	//			SetExplosion(pBillboard->pos, 0);
	//			SetParticle(pBillboard->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 60, 5.0f, 4);
	//		}
	//	}

	//	if (pWall->rot.y == D3DX_PI * 0.5f || pWall->rot.y == D3DX_PI * -0.5f)
	//	{
	//		if (
	//			(pBillboard->pos.x >= pWall->pos.x && pBillboard->posOld.x < pWall->pos.x ||
	//			pBillboard->pos.x <= pWall->pos.x && pBillboard->posOld.x > pWall->pos.x) &&
	//			pBillboard->pos.z >= pWall->pos.z - MAX_WIDTH &&
	//			pBillboard->pos.z <= pWall->pos.z + MAX_WIDTH &&
	//			pBillboard->pos.y >= pWall->pos.y &&
	//			pBillboard->pos.y <= pWall->pos.y + MAX_HEIGHT
	//			)
	//		{//敵と攻撃が当たった
	//			pBillboard->bUse = false;
	//			falseShadow(g_nIndexBillboard[nCnt]);
	//			SetExplosion(pBillboard->pos, 0);
	//			SetParticle(pBillboard->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),60,5.0f,4);
	//		}
	//	}
	//}
}

//====================================================================
//弾の所得
//====================================================================
Billboard * GetBillboard(void)
{
	return &g_aBillboard[0];
}