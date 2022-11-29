#include "main.h"
#include "object00.h"
#include "camera.h"
#include "billboard.h"
#include "wall.h"
#include "shadow.h"
#include "input.h"


#define MAX_OBJECT00 (128)		//�I�u�W�F�N�g�̍ő吔

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureObject00[100] = {};	//�e�N�X�`���̃|�C���^
LPD3DXMESH g_pMeshObject00 = NULL;					//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatObject00 = NULL;				//�}�e���A���ւ̃|�C���^
DWORD g_dwNumMatObject00 = 0;						//�}�e���A���̐�
Object00 g_Object00[MAX_OBJECT00];					//�I�u�W�F�N�g00�̏��
D3DXVECTOR3 g_Object00vtxMin;						//���f���̍ŏ�
D3DXVECTOR3 g_Object00vtxMax;						//���f���̍ő�
int g_nIndexObject00Shadow[MAX_OBJECT00];			//�e�̃C���f�b�N�X(�ԍ�)

//====================================================================
//�I�u�W�F�N�g00�̏���������
//====================================================================
void InitObject00(void)
{
	int nCntObject;

	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

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

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\fence.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatObject00,
		NULL,
		&g_dwNumMatObject00,
		&g_pMeshObject00);

	//���_��������
	nNumVtx = g_pMeshObject00->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y������
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshObject00->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshObject00->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//���_���W�̑��

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

			pVtxBuff += dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		}

	//���_�o�b�t�@���A�����b�N
	g_pMeshObject00->UnlockVertexBuffer();

	D3DXMATERIAL *pMat;	//�}�e���A���ւ̃|�C���^

	//�}�e���A�����ɑ΂���|�C���^������
	pMat = (D3DXMATERIAL*)g_pBuffMatObject00->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObject00; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureObject00[nCntMat]);
		}
	}

	SetObject00(D3DXVECTOR3(100.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject00(D3DXVECTOR3(-100.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

}

//====================================================================
//�I�u�W�F�N�g00�̏I������
//====================================================================
void UninitObject00(void)
{
	//���b�V���̔j��
	if (g_pMeshObject00 != NULL)
	{
		g_pMeshObject00->Release();
		g_pMeshObject00 = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatObject00 != NULL)
	{
		g_pBuffMatObject00->Release();
		g_pBuffMatObject00 = NULL;
	}
}

//====================================================================
//�I�u�W�F�N�g00�̍X�V����
//====================================================================
void UpdateObject00(void)
{

}

//====================================================================
//�I�u�W�F�N�g00�̕`�揈��
//====================================================================
void DrawObject00(void)
{
	int nCntObject;

	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	for (nCntObject = 0; nCntObject < MAX_OBJECT00; nCntObject++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Object00[nCntObject].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Object00[nCntObject].rot.y, g_Object00[nCntObject].rot.x, g_Object00[nCntObject].rot.z);

		D3DXMatrixMultiply(&g_Object00[nCntObject].mtxWorld, &g_Object00[nCntObject].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Object00[nCntObject].pos.x, g_Object00[nCntObject].pos.y, g_Object00[nCntObject].pos.z);

		D3DXMatrixMultiply(&g_Object00[nCntObject].mtxWorld, &g_Object00[nCntObject].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Object00[nCntObject].mtxWorld);

		//���݂̃}�e���A��������
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^����������
		pMat = (D3DXMATERIAL*)g_pBuffMatObject00->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObject00; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureObject00[nCntMat]);

			if (g_Object00[nCntObject].bUse == true)
			{
				//�I�u�W�F�N�g00(�p�[�c)�̕`��
				g_pMeshObject00->DrawSubset(nCntMat);
			}
		}
		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//====================================================================
//�r���{�[�h�̐ݒ菈��
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

			//�e�̐ݒ�
			g_nIndexObject00Shadow[nCntObject] = SetShadow();

			//�e�̈ʒu��ݒ�
			SetPositionShadow(g_nIndexObject00Shadow[nCntObject], g_Object00[nCntObject].pos);
			break;
		}
	}
}

//====================================================================
//�ǂ̏���
//====================================================================
Object00 * GetObject00(void)
{
	return &g_Object00[0];
}