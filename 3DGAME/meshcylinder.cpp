#include "main.h"
#include "meshcylinder.h"

#define WAIGHT_SIZE (17)			//横の頂点数(９or１７or３３)
#define HEIGHT_SIZE (6)				//縦の頂点数
#define WAIGHT_CENTER (0.5f)		//横の原点(0.0f～1.0f)
#define HEIGHT_CENTER (1.0f)		//縦の原点(0.0f～1.0f)
#define CYLINDER_HEIGHT (10.0f)		//壁一枚の高さ
#define CYLINDER_RADIUS (300.0f)	//円の半径

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder;				//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder;		//頂点バッファのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIndxBuffMeshCylinder;		//インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshCylinder;							//位置
D3DXVECTOR3 g_rotMeshCylinder;							//向き
D3DXMATRIX g_mtxWorldMeshCylinder;						//ワールドマトリックス

//====================================================================
//メッシュシリンダーの初期化処理
//====================================================================
void InitMeshCylinder(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ENEMY00.png",
		&g_pTextureMeshCylinder);

	g_posMeshCylinder = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshCylinder = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * HEIGHT_SIZE * WAIGHT_SIZE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);

	VERTEX_3D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);


	//pVtx[0].pos.x  = g_posMeshCylinder.x + sinf(D3DX_PI * 0.0f) * CYLINDER_RADIUS;
	//pVtx[1].pos.x  = g_posMeshCylinder.x + sinf(D3DX_PI * 0.25f) * CYLINDER_RADIUS;
	//pVtx[2].pos.x  = g_posMeshCylinder.x + sinf(D3DX_PI * 0.5f) * CYLINDER_RADIUS;
	//pVtx[3].pos.x  = g_posMeshCylinder.x + sinf(D3DX_PI * 0.75f) * CYLINDER_RADIUS;
	//pVtx[4].pos.x  = g_posMeshCylinder.x + sinf(D3DX_PI * 1.0f) * CYLINDER_RADIUS;
	//pVtx[5].pos.x  = g_posMeshCylinder.x + sinf(D3DX_PI * 1.25f) * CYLINDER_RADIUS;
	//pVtx[6].pos.x  = g_posMeshCylinder.x + sinf(D3DX_PI * 1.5f) * CYLINDER_RADIUS;
	//pVtx[7].pos.x  = g_posMeshCylinder.x + sinf(D3DX_PI * 1.75f) * CYLINDER_RADIUS;
	//pVtx[8].pos.x  = g_posMeshCylinder.x + sinf(D3DX_PI * 2.0f) * CYLINDER_RADIUS;
	//pVtx[9].pos.x  = g_posMeshCylinder.x + sinf(D3DX_PI * 0.0f) * CYLINDER_RADIUS;
	//pVtx[10].pos.x = g_posMeshCylinder.x + sinf(D3DX_PI * 0.25f) * CYLINDER_RADIUS;
	//pVtx[11].pos.x = g_posMeshCylinder.x + sinf(D3DX_PI * 0.5f) * CYLINDER_RADIUS;
	//pVtx[12].pos.x = g_posMeshCylinder.x + sinf(D3DX_PI * 0.75f) * CYLINDER_RADIUS;
	//pVtx[13].pos.x = g_posMeshCylinder.x + sinf(D3DX_PI * 1.0f) * CYLINDER_RADIUS;
	//pVtx[14].pos.x = g_posMeshCylinder.x + sinf(D3DX_PI * 1.25f) * CYLINDER_RADIUS;
	//pVtx[15].pos.x = g_posMeshCylinder.x + sinf(D3DX_PI * 1.5f) * CYLINDER_RADIUS;
	//pVtx[16].pos.x = g_posMeshCylinder.x + sinf(D3DX_PI * 1.75f) * CYLINDER_RADIUS;
	//pVtx[17].pos.x = g_posMeshCylinder.x + sinf(D3DX_PI * 2.0f) * CYLINDER_RADIUS;

	//pVtx[0].pos.y  = CYLINDER_HEIGHT;
	//pVtx[1].pos.y  = CYLINDER_HEIGHT;
	//pVtx[2].pos.y  = CYLINDER_HEIGHT;
	//pVtx[3].pos.y  = CYLINDER_HEIGHT;
	//pVtx[4].pos.y  = CYLINDER_HEIGHT;
	//pVtx[5].pos.y  = CYLINDER_HEIGHT;
	//pVtx[6].pos.y  = CYLINDER_HEIGHT;
	//pVtx[7].pos.y  = CYLINDER_HEIGHT;
	//pVtx[8].pos.y  = CYLINDER_HEIGHT;
	//pVtx[9].pos.y  = 0.0f;
	//pVtx[10].pos.y = 0.0f;
	//pVtx[11].pos.y = 0.0f;
	//pVtx[12].pos.y = 0.0f;
	//pVtx[13].pos.y = 0.0f;
	//pVtx[14].pos.y = 0.0f;
	//pVtx[15].pos.y = 0.0f;
	//pVtx[16].pos.y = 0.0f;
	//pVtx[17].pos.y = 0.0f;

	//pVtx[0].pos.z  = g_posMeshCylinder.z + cosf(D3DX_PI * 0.0f) * CYLINDER_RADIUS;
	//pVtx[1].pos.z  = g_posMeshCylinder.z + cosf(D3DX_PI * 0.25f) * CYLINDER_RADIUS;
	//pVtx[2].pos.z  = g_posMeshCylinder.z + cosf(D3DX_PI * 0.5f) * CYLINDER_RADIUS;
	//pVtx[3].pos.z  = g_posMeshCylinder.z + cosf(D3DX_PI * 0.75f) * CYLINDER_RADIUS;
	//pVtx[4].pos.z  = g_posMeshCylinder.z + cosf(D3DX_PI * 1.0f) * CYLINDER_RADIUS;
	//pVtx[5].pos.z  = g_posMeshCylinder.z + cosf(D3DX_PI * 1.25f) * CYLINDER_RADIUS;
	//pVtx[6].pos.z  = g_posMeshCylinder.z + cosf(D3DX_PI * 1.5f) * CYLINDER_RADIUS;
	//pVtx[7].pos.z  = g_posMeshCylinder.z + cosf(D3DX_PI * 1.75f) * CYLINDER_RADIUS;
	//pVtx[8].pos.z  = g_posMeshCylinder.z + cosf(D3DX_PI * 2.0f) * CYLINDER_RADIUS;
	//pVtx[9].pos.z  = g_posMeshCylinder.z + cosf(D3DX_PI * 0.0f) * CYLINDER_RADIUS;
	//pVtx[10].pos.z = g_posMeshCylinder.z + cosf(D3DX_PI * 0.25f) * CYLINDER_RADIUS;
	//pVtx[11].pos.z = g_posMeshCylinder.z + cosf(D3DX_PI * 0.5f) * CYLINDER_RADIUS;
	//pVtx[12].pos.z = g_posMeshCylinder.z + cosf(D3DX_PI * 0.75f) * CYLINDER_RADIUS;
	//pVtx[13].pos.z = g_posMeshCylinder.z + cosf(D3DX_PI * 1.0f) * CYLINDER_RADIUS;
	//pVtx[14].pos.z = g_posMeshCylinder.z + cosf(D3DX_PI * 1.25f) * CYLINDER_RADIUS;
	//pVtx[15].pos.z = g_posMeshCylinder.z + cosf(D3DX_PI * 1.5f) * CYLINDER_RADIUS;
	//pVtx[16].pos.z = g_posMeshCylinder.z + cosf(D3DX_PI * 1.75f) * CYLINDER_RADIUS;
	//pVtx[17].pos.z = g_posMeshCylinder.z + cosf(D3DX_PI * 2.0f) * CYLINDER_RADIUS;


	for (int nCnt = 0, nData = 0, nCenterW = 0, nCenterH = 0, nHeight = 0; nCnt < HEIGHT_SIZE * WAIGHT_SIZE; nCnt++)
	{
		//横番号の代入
		nCenterW = nCnt % WAIGHT_SIZE;

		if (nCnt % WAIGHT_SIZE == 0 && nCnt != 0)
		{
			nHeight++;
		}

		//頂点座標の設定 
		//if (nCnt < WAIGHT_SIZE)
		//{
		//	pVtx[nCnt].pos.x = g_posMeshCylinder.x + sinf(D3DX_PI * (1.0f / ((WAIGHT_SIZE - 1) / 2)) * nCnt) * CYLINDER_RADIUS;
		//}
		//else
		//{
		//	pVtx[nCnt].pos.x = g_posMeshCylinder.x + sinf(D3DX_PI * (1.0f / ((WAIGHT_SIZE - 1) / 2)) * (nCnt - 1)) * CYLINDER_RADIUS;
		//}

		pVtx[nCnt].pos.x = g_posMeshCylinder.x + sinf(D3DX_PI * (1.0f / ((WAIGHT_SIZE - 1) / 2)) * (nCnt - nHeight)) * CYLINDER_RADIUS;

		//if (nCnt < WAIGHT_SIZE)
		//{
		//	pVtx[nCnt].pos.y = CYLINDER_HEIGHT;
		//}
		//else
		//{
		//	pVtx[nCnt].pos.y = 0.0f;
		//}

		pVtx[nCnt].pos.y = CYLINDER_HEIGHT * ((HEIGHT_SIZE - 1) - nHeight);

		//if (nCnt < WAIGHT_SIZE)
		//{
		//	pVtx[nCnt].pos.z = g_posMeshCylinder.z + cosf(D3DX_PI * (1.0f / ((WAIGHT_SIZE - 1) / 2)) * nCnt) * CYLINDER_RADIUS;
		//}
		//else
		//{
		//	pVtx[nCnt].pos.z = g_posMeshCylinder.z + cosf(D3DX_PI * (1.0f / ((WAIGHT_SIZE - 1) / 2)) * (nCnt - 1)) * CYLINDER_RADIUS;
		//}

		pVtx[nCnt].pos.z = g_posMeshCylinder.z + cosf(D3DX_PI * (1.0f / ((WAIGHT_SIZE - 1) / 2)) * (nCnt - nHeight)) * CYLINDER_RADIUS;

		//法線ベクトルの設定
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		//頂点カラーの設定
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

		if (nCenterW % 2 == 0)
		{
			pVtx[nCnt].tex.x = 0.0f;
		}
		else
		{
			pVtx[nCnt].tex.x = 1.0f;
		}
		if (nCenterH % 2 == 0)
		{
			pVtx[nCnt].tex.y = 0.0f;
		}
		else
		{
			pVtx[nCnt].tex.y = 1.0f;
		}

		if ((nCnt - nCenterH) % (WAIGHT_SIZE - 1) == 0 && nCnt != 0 && nCnt != (WAIGHT_SIZE - 1) * nCenterH + nCenterH)
		{//縦番号の代入
			nCenterH++;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshCylinder->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (WAIGHT_SIZE * HEIGHT_SIZE + WAIGHT_SIZE * (HEIGHT_SIZE - 2) + 2 * (HEIGHT_SIZE - 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIndxBuffMeshCylinder,
		NULL);

	WORD *pIdx;	//インデックス情報へのポインタ

				//インデックスバッファをロックし、頂点番号データへのポインタを所得
	g_pIndxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0, nOri = 0; nCnt < (WAIGHT_SIZE * HEIGHT_SIZE + WAIGHT_SIZE * (HEIGHT_SIZE - 2) + 2 * (HEIGHT_SIZE - 2)) / 2; nCnt++)
	{
		if ((nCnt - nOri) % WAIGHT_SIZE == 0 && nCnt != 0 && nCnt != WAIGHT_SIZE * nOri + nOri)
		{
			pIdx[nCnt * 2] = nCnt - 1 - nOri;			//偶数
			pIdx[nCnt * 2 + 1] = nCnt + WAIGHT_SIZE - nOri;		//奇数

			nOri++;
		}
		else
		{
			pIdx[nCnt * 2] = nCnt + WAIGHT_SIZE - nOri;	//偶数
			pIdx[nCnt * 2 + 1] = nCnt - nOri;		//奇数
		}
	}

	//インデックスバッファをアンロックする
	g_pIndxBuffMeshCylinder->Unlock();
}

//====================================================================
//メッシュシリンダーの終了処理
//====================================================================
void UninitMeshCylinder(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIndxBuffMeshCylinder != NULL)
	{
		g_pIndxBuffMeshCylinder->Release();
		g_pIndxBuffMeshCylinder = NULL;
	}
}

//====================================================================
//メッシュシリンダーの更新処理
//====================================================================
void UpdateMeshCylinder(void)
{

}

//====================================================================
//メッシュシリンダーの描画処理
//====================================================================
void DrawMeshCylinder(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

									//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshCylinder);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshCylinder.y, g_rotMeshCylinder.x, g_rotMeshCylinder.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshCylinder, &g_mtxWorldMeshCylinder, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshCylinder.x, g_posMeshCylinder.y, g_posMeshCylinder.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshCylinder, &g_mtxWorldMeshCylinder, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshCylinder);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIndxBuffMeshCylinder);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		0,//用意した頂点の数
		0,
		(WAIGHT_SIZE * HEIGHT_SIZE + WAIGHT_SIZE * (HEIGHT_SIZE - 2) + 2 * (HEIGHT_SIZE - 2)) - 2);//描画するプリミティブの数
}