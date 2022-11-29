#include "main.h"
#include "model.h"
#include "camera.h"
#include "billboard.h"
#include "wall.h"
#include "shadow.h"
#include "input.h"
#include "debugproc.h"

#define MODEL_SPEED (2.5f)	//モデルのスピード
#define MODEL_ROT_SPEED (0.2f)	//モデルの回転スピード

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureModel[100] = {};	//テクスチャのポインタ
LPD3DXMESH g_pMeshModel = NULL;					//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatModel = NULL;			//マテリアルへのポインタ
DWORD g_dwNumMatModel = 0;						//マテリアルの数
Model g_Model;									//モデルの情報
int g_nIndexModelShadow = -1;						//影のインデックス(番号)

//====================================================================
//モデルの初期化処理
//====================================================================
void InitModel(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	g_Model.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Model.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Model.vecMove = g_Model.posOld - g_Model.pos;
	g_Model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Model.g_vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	g_Model.g_vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

	//Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\kamado.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_dwNumMatModel,
		&g_pMeshModel);

	//頂点数を所得
	nNumVtx = g_pMeshModel->GetNumVertices();

	//頂点フォーマットのサイズを所得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//頂点バッファをロック
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

		if (g_Model.g_vtxMin.x > vtx.x)
		{
			g_Model.g_vtxMin.x = vtx.x;
		}
		if (g_Model.g_vtxMin.y > vtx.y)
		{
			g_Model.g_vtxMin.y = vtx.y;
		}
		if (g_Model.g_vtxMin.z > vtx.z)
		{
			g_Model.g_vtxMin.z = vtx.z;
		}

		if (g_Model.g_vtxMax.x < vtx.x)
		{
			g_Model.g_vtxMax.x = vtx.x;
		}
		if (g_Model.g_vtxMax.y < vtx.y)
		{
			g_Model.g_vtxMax.y = vtx.y;
		}
		if (g_Model.g_vtxMax.z < vtx.z)
		{
			g_Model.g_vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	//頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	g_pMeshModel->UnlockVertexBuffer();

	D3DXMATERIAL *pMat;	//マテリアルへのポインタ

	//マテリアル情報に対するポインタを所得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		if(pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureModel[nCntMat]);
		}
	}

	//影の設定
	g_nIndexModelShadow = SetShadow();
}

//====================================================================
//モデルの終了処理
//====================================================================
void UninitModel(void)
{
	//メッシュの破棄
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//====================================================================
//モデルの更新処理
//====================================================================
void UpdateModel(void)
{
	g_Model.posOld = g_Model.pos;

	//減衰係数
	g_Model.move = g_Model.move * 0.5f;

	//値の切り捨て
	if (g_Model.move.x <= 0.005f && g_Model.move.x >= -0.005f)
	{
		g_Model.move.x = 0.0f;
	}
	if (g_Model.move.z <= 0.005f && g_Model.move.z >= -0.005f)
	{
		g_Model.move.z = 0.0f;
	}

	//左スティックによる前後移動	
	g_Model.move.z += GetGamepad_Stick_Left(0).y * cosf(Getrot().y);
	g_Model.move.x += GetGamepad_Stick_Left(0).y * sinf(Getrot().y);

	//左スティックによる左右移動
	g_Model.move.x += GetGamepad_Stick_Left(0).x * cosf(Getrot().y);
	g_Model.move.z -= GetGamepad_Stick_Left(0).x * sinf(Getrot().y);


	//キーボードの上下移動処理
	if (GetKeyboardPress(DIK_RSHIFT) == true)
	{
		g_Model.move.y++;
	}
	if (GetKeyboardPress(DIK_RCONTROL) == true)
	{
		g_Model.move.y--;
	}

	//キーボードの移動処理
	if (GetKeyboardPress(DIK_W) == true)
	{
		g_Model.move.z += 1.0f * cosf(Getrot().y) * MODEL_SPEED;
		g_Model.move.x += 1.0f * sinf(Getrot().y) * MODEL_SPEED;

	}
	if (GetKeyboardPress(DIK_S) == true)
	{
		g_Model.move.z += -1.0f * cosf(Getrot().y) * MODEL_SPEED;
		g_Model.move.x += -1.0f * sinf(Getrot().y) * MODEL_SPEED;
	}
	if (GetKeyboardPress(DIK_A) == true)
	{
		g_Model.move.x += -1.0f * cosf(Getrot().y) * MODEL_SPEED;
		g_Model.move.z -= -1.0f * sinf(Getrot().y) * MODEL_SPEED;

	}
	if (GetKeyboardPress(DIK_D) == true)
	{
		g_Model.move.x += 1.0f * cosf(Getrot().y) * MODEL_SPEED;
		g_Model.move.z -= 1.0f * sinf(Getrot().y) * MODEL_SPEED;
	}

	//位置更新(入力による動き)
	g_Model.pos += g_Model.move;

	//移動ベクトルの更新
	g_Model.vecMove = g_Model.posOld - g_Model.pos;

	//移動方向に向きを合わせる処理
	float fRotMove, fRotDest, fRotDiff;

	fRotMove = g_Model.rot.y;
	fRotDest = Getrot().y;

	if (GetKeyboardPress(DIK_E) == true)
	{
		g_Model.rot.y += 0.1f;
	}
	if (GetKeyboardPress(DIK_Q) == true)
	{
		g_Model.rot.y -= 0.1f;
	}

	if (GetKeyboardPress(DIK_W) == true)
	{
		fRotMove = atan2f(sinf(g_Model.rot.y), cosf(g_Model.rot.y));
		fRotDest = atan2f(sinf(Getrot().y), cosf(Getrot().y));
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		fRotMove = atan2f(sinf(g_Model.rot.y), cosf(g_Model.rot.y));
		fRotDest = atan2f(sinf(Getrot().y), cosf(Getrot().y)) + D3DX_PI * 0.5f;
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		fRotMove = atan2f(sinf(g_Model.rot.y), cosf(g_Model.rot.y));
		fRotDest = atan2f(sinf(Getrot().y), cosf(Getrot().y)) + D3DX_PI * -0.5f;
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		fRotMove = atan2f(sinf(g_Model.rot.y), cosf(g_Model.rot.y));
		fRotDest = atan2f(sinf(Getrot().y), cosf(Getrot().y)) + D3DX_PI;
	}

	fRotDiff = fRotDest - fRotMove;

	if (fRotDiff > D3DX_PI * 1.0f)
	{
		fRotDiff -= D3DX_PI * 2.0f;
	}
	else if (fRotDiff < -D3DX_PI * 1.0f)
	{
		fRotDiff += D3DX_PI * 2.0f;
	}

	fRotMove += fRotDiff * MODEL_ROT_SPEED;

	if (fRotMove > D3DX_PI * 1.0f)
	{
		fRotMove -= D3DX_PI * 2.0f;
	}
	else if (fRotMove < -D3DX_PI * 1.0f)
	{
		fRotMove += D3DX_PI * 2.0f;
	}

	if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_D) == true)
	{
		g_Model.rot.y = fRotMove;
	}

	//弾発射処理
	if (GetKeyboardTrigger(DIK_SPACE) == true || GetGamepadTrigger(BUTTON_B, 0) == true)
	{
		SetBillboard(D3DXVECTOR3(g_Model.pos.x, g_Model.pos.y + 20.0f, g_Model.pos.z), g_Model.rot.y);
	}

	//壁との当たり判定
	CollisionWall(&g_Model.pos, &g_Model.posOld, g_Model.g_vtxMin, g_Model.g_vtxMax);

	//一周した時の向きの補正
	if (g_Model.rot.y > D3DX_PI * 1.0f)
	{
		g_Model.rot.y -= D3DX_PI * 2.0f;
	}
	else if (g_Model.rot.y < -D3DX_PI * 1.0f)
	{
		g_Model.rot.y += D3DX_PI * 2.0f;
	}

	//影の位置を設定
	SetPositionShadow(g_nIndexModelShadow, g_Model.pos);

	PrintDebugProc("fff", g_Model.pos.x, g_Model.pos.y, g_Model.pos.z);
}

//====================================================================
//モデルの描画処理
//====================================================================
void DrawModel(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Model.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model.rot.y, g_Model.rot.x, g_Model.rot.z);

	D3DXMatrixMultiply(&g_Model.mtxWorld, &g_Model.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Model.pos.x, g_Model.pos.y, g_Model.pos.z);

	D3DXMatrixMultiply(&g_Model.mtxWorld, &g_Model.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Model.mtxWorld);

	//現在のマテリアルを所得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを所得する
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureModel[nCntMat]);

		//モデル(パーツ)の描画
		g_pMeshModel->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
//====================================================================
//壁の所得
//====================================================================
Model * GetModel(void)
{
	return &g_Model;
}