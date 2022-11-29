#include "main.h"
#include "meshfieldidx.h"

#define WAIGHT_SIZE (5)			//横の頂点数
#define HEIGHT_SIZE (3)			//縦の頂点数
#define WAIGHT_CENTER (0.5f)	//横の原点(0.0f～1.0f)
#define HEIGHT_CENTER (0.5f)	//縦の原点(0.0f～1.0f)
#define FIELD_SIZE (25.0f)		//床一枚の大きさ
#define MESHFIELD_SIZE (50.0f)	//影の大きさ

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureMeshField;				//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField;		//頂点バッファのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIndxBuffMeshField;		//インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshField;							//位置
D3DXVECTOR3 g_rotMeshField;							//向き
D3DXMATRIX g_mtxWorldMeshField;						//ワールドマトリックス

//====================================================================
//メッシュフィールドの初期化処理
//====================================================================
void InitMeshFieldIdx(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ENEMY00.png",
		&g_pTextureMeshField);

	g_posMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * HEIGHT_SIZE * WAIGHT_SIZE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	VERTEX_3D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0, nCenterW = 0, nCenterH = 0; nCnt < HEIGHT_SIZE * WAIGHT_SIZE; nCnt++)
	{
		nCenterW = nCnt % WAIGHT_SIZE;

		//頂点座標の設定 
		pVtx[nCnt].pos.x = FIELD_SIZE * (nCenterW - (((float)WAIGHT_SIZE - 1) * WAIGHT_CENTER));

		pVtx[nCnt].pos.y = 0.0f;

		pVtx[nCnt].pos.z = FIELD_SIZE * (HEIGHT_SIZE - 1) * HEIGHT_CENTER - FIELD_SIZE * nCenterH;

		//法線ベクトルの設定
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
		{
			nCenterH++;
		}
	}
		//頂点座標の設定 
		//pVtx[0].pos = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
		//pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
		//pVtx[2].pos = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
		//pVtx[3].pos = D3DXVECTOR3(-100.0f, 0.0f, 50.0f);
		//pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f);
		//pVtx[5].pos = D3DXVECTOR3(100.0f, 0.0f, 50.0f);
		//pVtx[6].pos = D3DXVECTOR3(-100.0f, 0.0f, -50.0f);
		//pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
		//pVtx[8].pos = D3DXVECTOR3(100.0f, 0.0f, -50.0f);
		//pVtx[9].pos = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
		//pVtx[10].pos = D3DXVECTOR3(0.0f, 0.0f, -100.0f);
		//pVtx[11].pos = D3DXVECTOR3(100.0f, 0.0f, -100.0f);

		//pVtx[0].pos =  D3DXVECTOR3(-200.0f, 0.0f, 100.0f);
		//pVtx[1].pos =  D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
		//pVtx[2].pos =  D3DXVECTOR3(+100.0f, 0.0f, 100.0f);
		//pVtx[3].pos =  D3DXVECTOR3(+200.0f, 0.0f, 100.0f);
		//pVtx[4].pos =  D3DXVECTOR3(-200.0f, 0.0f, 0.0f);
		//pVtx[5].pos =  D3DXVECTOR3(-100.0f, 0.0f, 0.0f);
		//pVtx[6].pos =  D3DXVECTOR3(+100.0f, 0.0f, 0.0f);
		//pVtx[7].pos =  D3DXVECTOR3(+200.0f, 0.0f, 0.0f);
		//pVtx[8].pos =  D3DXVECTOR3(-200.0f, 0.0f, -100.0f);
		//pVtx[9].pos =  D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
		//pVtx[10].pos = D3DXVECTOR3(+100.0f, 0.0f, -100.0f);
		//pVtx[11].pos = D3DXVECTOR3(+200.0f, 0.0f, -100.0f);

		//pVtx[0].pos.x = -200.0f;
		//pVtx[1].pos.x = -100.0f;
		//pVtx[2].pos.x = 100.0f;
		//pVtx[3].pos.x = 200.0f;
		//pVtx[4].pos.x = -200.0f;
		//pVtx[5].pos.x = -100.0f;
		//pVtx[6].pos.x = 100.0f;
		//pVtx[7].pos.x = 200.0f;
		//pVtx[8].pos.x = -200.0f;
		//pVtx[9].pos.x = -100.0f;
		//pVtx[10].pos.x= 100.0f;
		//pVtx[11].pos.x= 200.0f;

		//pVtx[0].pos.z =	100.0f;
		//pVtx[1].pos.z =	100.0f;
		//pVtx[2].pos.z =	100.0f;
		//pVtx[3].pos.z =	100.0f;
		//pVtx[4].pos.z =	0.0f;
		//pVtx[5].pos.z =	0.0f;
		//pVtx[6].pos.z =	0.0f;
		//pVtx[7].pos.z =	0.0f;
		//pVtx[8].pos.z =	-100.0f;
		//pVtx[9].pos.z =	-100.0f;
		//pVtx[10].pos.z= -100.0f;
		//pVtx[11].pos.z= -100.0f;

	////テクスチャ座標の設定
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[4].tex = D3DXVECTOR2(1.0f, 1.0f);
	//pVtx[5].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[6].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[7].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffMeshField->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (WAIGHT_SIZE * HEIGHT_SIZE + WAIGHT_SIZE * (HEIGHT_SIZE - 2) + 2 * (HEIGHT_SIZE - 2)),
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,
								D3DPOOL_MANAGED,
								&g_pIndxBuffMeshField,
								NULL);

	WORD *pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを所得
	g_pIndxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	////頂点データの設定
	//pIdx[0] = 3;
	//pIdx[1] = 0;
	//pIdx[2] = 4;
	//pIdx[3] = 1;
	//pIdx[4] = 5;
	//pIdx[5] = 2;
	//pIdx[6] = 2;
	//pIdx[7] = 6;
	//pIdx[8] = 6;
	//pIdx[9] = 3;
	//pIdx[10] = 7;
	//pIdx[11] = 4;
	//pIdx[12] = 8;
	//pIdx[13] = 5;

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
	g_pIndxBuffMeshField->Unlock();
}

//====================================================================
//メッシュフィールドの終了処理
//====================================================================
void UninitMeshFieldIdx(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIndxBuffMeshField != NULL)
	{
		g_pIndxBuffMeshField->Release();
		g_pIndxBuffMeshField = NULL;
	}
}

//====================================================================
//メッシュフィールドの更新処理
//====================================================================
void UpdateMeshFieldIdx(void)
{

}

//====================================================================
//メッシュフィールドの描画処理
//====================================================================
void DrawMeshFieldIdx(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

									//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIndxBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshField);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		0,//用意した頂点の数
		0,
		(WAIGHT_SIZE * HEIGHT_SIZE + WAIGHT_SIZE * (HEIGHT_SIZE - 2) + 2 * (HEIGHT_SIZE - 2)) - 2);//描画するプリミティブの数
}