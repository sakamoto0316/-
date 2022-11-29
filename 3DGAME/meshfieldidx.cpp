#include "main.h"
#include "meshfieldidx.h"

#define WAIGHT_SIZE (5)			//���̒��_��
#define HEIGHT_SIZE (3)			//�c�̒��_��
#define WAIGHT_CENTER (0.5f)	//���̌��_(0.0f�`1.0f)
#define HEIGHT_CENTER (0.5f)	//�c�̌��_(0.0f�`1.0f)
#define FIELD_SIZE (25.0f)		//���ꖇ�̑傫��
#define MESHFIELD_SIZE (50.0f)	//�e�̑傫��

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureMeshField;				//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField;		//���_�o�b�t�@�̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIndxBuffMeshField;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posMeshField;							//�ʒu
D3DXVECTOR3 g_rotMeshField;							//����
D3DXMATRIX g_mtxWorldMeshField;						//���[���h�}�g���b�N�X

//====================================================================
//���b�V���t�B�[���h�̏���������
//====================================================================
void InitMeshFieldIdx(void)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ENEMY00.png",
		&g_pTextureMeshField);

	g_posMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * HEIGHT_SIZE * WAIGHT_SIZE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	VERTEX_3D*pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0, nCenterW = 0, nCenterH = 0; nCnt < HEIGHT_SIZE * WAIGHT_SIZE; nCnt++)
	{
		nCenterW = nCnt % WAIGHT_SIZE;

		//���_���W�̐ݒ� 
		pVtx[nCnt].pos.x = FIELD_SIZE * (nCenterW - (((float)WAIGHT_SIZE - 1) * WAIGHT_CENTER));

		pVtx[nCnt].pos.y = 0.0f;

		pVtx[nCnt].pos.z = FIELD_SIZE * (HEIGHT_SIZE - 1) * HEIGHT_CENTER - FIELD_SIZE * nCenterH;

		//�@���x�N�g���̐ݒ�
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
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
		//���_���W�̐ݒ� 
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

	////�e�N�X�`�����W�̐ݒ�
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[4].tex = D3DXVECTOR2(1.0f, 1.0f);
	//pVtx[5].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[6].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[7].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshField->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * (WAIGHT_SIZE * HEIGHT_SIZE + WAIGHT_SIZE * (HEIGHT_SIZE - 2) + 2 * (HEIGHT_SIZE - 2)),
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,
								D3DPOOL_MANAGED,
								&g_pIndxBuffMeshField,
								NULL);

	WORD *pIdx;	//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^������
	g_pIndxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	////���_�f�[�^�̐ݒ�
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
			pIdx[nCnt * 2] = nCnt - 1 - nOri;			//����
			pIdx[nCnt * 2 + 1] = nCnt + WAIGHT_SIZE - nOri;		//�

			nOri++;
		}
		else
		{
			pIdx[nCnt * 2] = nCnt + WAIGHT_SIZE - nOri;	//����
			pIdx[nCnt * 2 + 1] = nCnt - nOri;		//�
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIndxBuffMeshField->Unlock();
}

//====================================================================
//���b�V���t�B�[���h�̏I������
//====================================================================
void UninitMeshFieldIdx(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIndxBuffMeshField != NULL)
	{
		g_pIndxBuffMeshField->Release();
		g_pIndxBuffMeshField = NULL;
	}
}

//====================================================================
//���b�V���t�B�[���h�̍X�V����
//====================================================================
void UpdateMeshFieldIdx(void)
{

}

//====================================================================
//���b�V���t�B�[���h�̕`�揈��
//====================================================================
void DrawMeshFieldIdx(void)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

									//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIndxBuffMeshField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshField);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		0,//�p�ӂ������_�̐�
		0,
		(WAIGHT_SIZE * HEIGHT_SIZE + WAIGHT_SIZE * (HEIGHT_SIZE - 2) + 2 * (HEIGHT_SIZE - 2)) - 2);//�`�悷��v���~�e�B�u�̐�
}