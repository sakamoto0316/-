#include "main.h"
#include "object00.h"
#include "camera.h"
#include "billboard.h"
#include "wall.h"
#include "shadow.h"
#include "input.h"


#define MAX_OBJECT00 (128)		//オブジェクトの最大数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureObject00[100] = {};	//テクスチャのポインタ
LPD3DXMESH g_pMeshObject00 = NULL;					//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatObject00 = NULL;				//マテリアルへのポインタ
DWORD g_dwNumMatObject00 = 0;						//マテリアルの数
Object00 g_Object00[MAX_OBJECT00];					//オブジェクト00の情報
D3DXVECTOR3 g_Object00vtxMin;						//モデルの最小
D3DXVECTOR3 g_Object00vtxMax;						//モデルの最大
int g_nIndexObject00Shadow[MAX_OBJECT00];			//影のインデックス(番号)

//====================================================================
//オブジェクト00の初期化処理
//====================================================================
void InitObject00(void)
{
	int nCntObject;

	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	for (nCntObject = 0; nCntObject < MAX_OBJECT00; nCntObject++)
	{
		g_Object00[nCntObject].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Object00[nCntObject].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Object00[nCntObject].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Object00[nCntObject].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Object00[nCntObject].bUse = false;
	}

	g_Object00vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	g_Object00vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

	//Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\fence.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatObject00,
		NULL,
		&g_dwNumMatObject00,
		&g_pMeshObject00);

	//頂点数を所得
	nNumVtx = g_pMeshObject00->GetNumVertices();

	//頂点フォーマットのサイズを所得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshObject00->GetFVF());

	//頂点バッファをロック
	g_pMeshObject00->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

			if (g_Object00vtxMin.x > vtx.x)
			{
				g_Object00vtxMin.x = vtx.x;
			}
			if (g_Object00vtxMin.y > vtx.y)
			{
				g_Object00vtxMin.y = vtx.y;
			}
			if (g_Object00vtxMin.z > vtx.z)
			{
				g_Object00vtxMin.z = vtx.z;
			}

			if (g_Object00vtxMax.x < vtx.x)
			{
				g_Object00vtxMax.x = vtx.x;
			}
			if (g_Object00vtxMax.y < vtx.y)
			{
				g_Object00vtxMax.y = vtx.y;
			}
			if (g_Object00vtxMax.z < vtx.z)
			{
				g_Object00vtxMax.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;	//頂点フォーマットのサイズ分ポインタを進める
		}

	//頂点バッファをアンロック
	g_pMeshObject00->UnlockVertexBuffer();

	D3DXMATERIAL *pMat;	//マテリアルへのポインタ

	//マテリアル情報に対するポインタを所得
	pMat = (D3DXMATERIAL*)g_pBuffMatObject00->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObject00; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureObject00[nCntMat]);
		}
	}

	SetObject00(D3DXVECTOR3(100.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject00(D3DXVECTOR3(-100.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

}

//====================================================================
//オブジェクト00の終了処理
//====================================================================
void UninitObject00(void)
{
	//メッシュの破棄
	if (g_pMeshObject00 != NULL)
	{
		g_pMeshObject00->Release();
		g_pMeshObject00 = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatObject00 != NULL)
	{
		g_pBuffMatObject00->Release();
		g_pBuffMatObject00 = NULL;
	}
}

//====================================================================
//オブジェクト00の更新処理
//====================================================================
void UpdateObject00(void)
{

}

//====================================================================
//オブジェクト00の描画処理
//====================================================================
void DrawObject00(void)
{
	int nCntObject;

	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	for (nCntObject = 0; nCntObject < MAX_OBJECT00; nCntObject++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Object00[nCntObject].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Object00[nCntObject].rot.y, g_Object00[nCntObject].rot.x, g_Object00[nCntObject].rot.z);

		D3DXMatrixMultiply(&g_Object00[nCntObject].mtxWorld, &g_Object00[nCntObject].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Object00[nCntObject].pos.x, g_Object00[nCntObject].pos.y, g_Object00[nCntObject].pos.z);

		D3DXMatrixMultiply(&g_Object00[nCntObject].mtxWorld, &g_Object00[nCntObject].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Object00[nCntObject].mtxWorld);

		//現在のマテリアルを所得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを所得する
		pMat = (D3DXMATERIAL*)g_pBuffMatObject00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObject00; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureObject00[nCntMat]);

			if (g_Object00[nCntObject].bUse == true)
			{
				//オブジェクト00(パーツ)の描画
				g_pMeshObject00->DrawSubset(nCntMat);
			}
		}
		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//====================================================================
//ビルボードの設定処理
//====================================================================
void SetObject00(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot)
{
	int nCntObject;

	for (nCntObject = 0; nCntObject < MAX_OBJECT00; nCntObject++)
	{
		if (g_Object00[nCntObject].bUse == false)
		{
			g_Object00[nCntObject].pos = pos;
			g_Object00[nCntObject].posOld = pos;
			g_Object00[nCntObject].move = move;
			g_Object00[nCntObject].rot = rot;
			g_Object00[nCntObject].bUse = true;

			//影の設定
			g_nIndexObject00Shadow[nCntObject] = SetShadow();

			//影の位置を設定
			SetPositionShadow(g_nIndexObject00Shadow[nCntObject], g_Object00[nCntObject].pos);
			break;
		}
	}
}

//====================================================================
//壁の所得
//====================================================================
Object00 * GetObject00(void)
{
	return &g_Object00[0];
}