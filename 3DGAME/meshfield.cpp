#include "main.h"
#include "meshfield.h"

#define HEIGHT_SIZE (2)	//縦の数
#define WAIGHT_SIZE (3)	//横の数
#define MESHFIELD_SIZE (50.0f)	//影の大きさ

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureMeshField00;				//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField00;			//頂点バッファのポインタ
D3DXVECTOR3 g_posMeshField00;							//位置
D3DXVECTOR3 g_rotMeshField00;							//向き
D3DXMATRIX g_mtxWorldMeshField00;						//ワールドマトリックス

//====================================================================
//メッシュフィールドの初期化処理
//====================================================================
void InitMeshField00(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	////テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\TEXTURE\\ENEMY00.png",
	//	&g_pTextureMeshField00);

	g_posMeshField00 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField00 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 14,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField00,
		NULL);

	VERTEX_3D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	g_pVtxBuffMeshField00->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	pVtx[0].pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
	pVtx[4].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	pVtx[6].pos = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	pVtx[7].pos = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
	pVtx[8].pos = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
	pVtx[9].pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(0.0f, 0.0f, -100.0f);
	pVtx[11].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[12].pos = D3DXVECTOR3(100.0f, 0.0f, -100.0f);
	pVtx[13].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);


	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[9].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[10].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[11].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[12].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[13].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffMeshField00->Unlock();
}

//====================================================================
//メッシュフィールドの終了処理
//====================================================================
void UninitMeshField00(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshField00 != NULL)
	{
		g_pTextureMeshField00->Release();
		g_pTextureMeshField00 = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshField00 != NULL)
	{
		g_pVtxBuffMeshField00->Release();
		g_pVtxBuffMeshField00 = NULL;
	}
}

//====================================================================
//メッシュフィールドの更新処理
//====================================================================
void UpdateMeshField00(void)
{

}

//====================================================================
//メッシュフィールドの描画処理
//====================================================================
void DrawMeshField00(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

									//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshField00);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshField00.y, g_rotMeshField00.x, g_rotMeshField00.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField00, &g_mtxWorldMeshField00, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshField00.x, g_posMeshField00.y, g_posMeshField00.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField00, &g_mtxWorldMeshField00, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField00);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField00, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshField00);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,										//プリミティブ(ポリゴン)数
		12);
}