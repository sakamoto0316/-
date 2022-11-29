#include "main.h"
#include "meshfield.h"

#define HEIGHT_SIZE (2)	//�c�̐�
#define WAIGHT_SIZE (3)	//���̐�
#define MESHFIELD_SIZE (50.0f)	//�e�̑傫��

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureMeshField00;				//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField00;			//���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posMeshField00;							//�ʒu
D3DXVECTOR3 g_rotMeshField00;							//����
D3DXMATRIX g_mtxWorldMeshField00;						//���[���h�}�g���b�N�X

//====================================================================
//���b�V���t�B�[���h�̏���������
//====================================================================
void InitMeshField00(void)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	////�e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\TEXTURE\\ENEMY00.png",
	//	&g_pTextureMeshField00);

	g_posMeshField00 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField00 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 14,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField00,
		NULL);

	VERTEX_3D*pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	g_pVtxBuffMeshField00->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ� 
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


	//�@���x�N�g���̐ݒ�
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

	//���_�J���[�̐ݒ�
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

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshField00->Unlock();
}

//====================================================================
//���b�V���t�B�[���h�̏I������
//====================================================================
void UninitMeshField00(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshField00 != NULL)
	{
		g_pTextureMeshField00->Release();
		g_pTextureMeshField00 = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshField00 != NULL)
	{
		g_pVtxBuffMeshField00->Release();
		g_pVtxBuffMeshField00 = NULL;
	}
}

//====================================================================
//���b�V���t�B�[���h�̍X�V����
//====================================================================
void UpdateMeshField00(void)
{

}

//====================================================================
//���b�V���t�B�[���h�̕`�揈��
//====================================================================
void DrawMeshField00(void)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

									//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshField00);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshField00.y, g_rotMeshField00.x, g_rotMeshField00.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField00, &g_mtxWorldMeshField00, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posMeshField00.x, g_posMeshField00.y, g_posMeshField00.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField00, &g_mtxWorldMeshField00, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField00);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField00, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshField00);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,										//�v���~�e�B�u(�|���S��)��
		12);
}