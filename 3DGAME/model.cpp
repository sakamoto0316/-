#include "main.h"
#include "model.h"
#include "camera.h"
#include "billboard.h"
#include "wall.h"
#include "shadow.h"
#include "input.h"
#include "debugproc.h"

#define MODEL_SPEED (2.5f)	//���f���̃X�s�[�h
#define MODEL_ROT_SPEED (0.2f)	//���f���̉�]�X�s�[�h

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureModel[100] = {};	//�e�N�X�`���̃|�C���^
LPD3DXMESH g_pMeshModel = NULL;					//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatModel = NULL;			//�}�e���A���ւ̃|�C���^
DWORD g_dwNumMatModel = 0;						//�}�e���A���̐�
Model g_Model;									//���f���̏��
int g_nIndexModelShadow = -1;						//�e�̃C���f�b�N�X(�ԍ�)

//====================================================================
//���f���̏���������
//====================================================================
void InitModel(void)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	g_Model.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Model.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Model.vecMove = g_Model.posOld - g_Model.pos;
	g_Model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Model.g_vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	g_Model.g_vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\kamado.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_dwNumMatModel,
		&g_pMeshModel);

	//���_��������
	nNumVtx = g_pMeshModel->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y������
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//���_���W�̑��

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

		pVtxBuff += dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_pMeshModel->UnlockVertexBuffer();

	D3DXMATERIAL *pMat;	//�}�e���A���ւ̃|�C���^

	//�}�e���A�����ɑ΂���|�C���^������
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		if(pMat[nCntMat].pTextureFilename != NULL)
		{
			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureModel[nCntMat]);
		}
	}

	//�e�̐ݒ�
	g_nIndexModelShadow = SetShadow();
}

//====================================================================
//���f���̏I������
//====================================================================
void UninitModel(void)
{
	//���b�V���̔j��
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//====================================================================
//���f���̍X�V����
//====================================================================
void UpdateModel(void)
{
	g_Model.posOld = g_Model.pos;

	//�����W��
	g_Model.move = g_Model.move * 0.5f;

	//�l�̐؂�̂�
	if (g_Model.move.x <= 0.005f && g_Model.move.x >= -0.005f)
	{
		g_Model.move.x = 0.0f;
	}
	if (g_Model.move.z <= 0.005f && g_Model.move.z >= -0.005f)
	{
		g_Model.move.z = 0.0f;
	}

	//���X�e�B�b�N�ɂ��O��ړ�	
	g_Model.move.z += GetGamepad_Stick_Left(0).y * cosf(Getrot().y);
	g_Model.move.x += GetGamepad_Stick_Left(0).y * sinf(Getrot().y);

	//���X�e�B�b�N�ɂ�鍶�E�ړ�
	g_Model.move.x += GetGamepad_Stick_Left(0).x * cosf(Getrot().y);
	g_Model.move.z -= GetGamepad_Stick_Left(0).x * sinf(Getrot().y);


	//�L�[�{�[�h�̏㉺�ړ�����
	if (GetKeyboardPress(DIK_RSHIFT) == true)
	{
		g_Model.move.y++;
	}
	if (GetKeyboardPress(DIK_RCONTROL) == true)
	{
		g_Model.move.y--;
	}

	//�L�[�{�[�h�̈ړ�����
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

	//�ʒu�X�V(���͂ɂ�铮��)
	g_Model.pos += g_Model.move;

	//�ړ��x�N�g���̍X�V
	g_Model.vecMove = g_Model.posOld - g_Model.pos;

	//�ړ������Ɍ��������킹�鏈��
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

	//�e���ˏ���
	if (GetKeyboardTrigger(DIK_SPACE) == true || GetGamepadTrigger(BUTTON_B, 0) == true)
	{
		SetBillboard(D3DXVECTOR3(g_Model.pos.x, g_Model.pos.y + 20.0f, g_Model.pos.z), g_Model.rot.y);
	}

	//�ǂƂ̓����蔻��
	CollisionWall(&g_Model.pos, &g_Model.posOld, g_Model.g_vtxMin, g_Model.g_vtxMax);

	//����������̌����̕␳
	if (g_Model.rot.y > D3DX_PI * 1.0f)
	{
		g_Model.rot.y -= D3DX_PI * 2.0f;
	}
	else if (g_Model.rot.y < -D3DX_PI * 1.0f)
	{
		g_Model.rot.y += D3DX_PI * 2.0f;
	}

	//�e�̈ʒu��ݒ�
	SetPositionShadow(g_nIndexModelShadow, g_Model.pos);

	PrintDebugProc("fff", g_Model.pos.x, g_Model.pos.y, g_Model.pos.z);
}

//====================================================================
//���f���̕`�揈��
//====================================================================
void DrawModel(void)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Model.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model.rot.y, g_Model.rot.x, g_Model.rot.z);

	D3DXMatrixMultiply(&g_Model.mtxWorld, &g_Model.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Model.pos.x, g_Model.pos.y, g_Model.pos.z);

	D3DXMatrixMultiply(&g_Model.mtxWorld, &g_Model.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Model.mtxWorld);

	//���݂̃}�e���A��������
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^����������
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureModel[nCntMat]);

		//���f��(�p�[�c)�̕`��
		g_pMeshModel->DrawSubset(nCntMat);
	}
	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}
//====================================================================
//�ǂ̏���
//====================================================================
Model * GetModel(void)
{
	return &g_Model;
}