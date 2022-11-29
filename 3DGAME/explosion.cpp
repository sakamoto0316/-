#include "main.h"
#include "explosion.h"

//�}�N����`
#define MAX_EXPLOSION (128)		//�����̍ő吔
#define SIZE_EXPLOSION (10.0f)	//�����̑傫��

//�����\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//���݈ʒu
	D3DXVECTOR3 posOld;				//�P�t���[���O�̈ʒu
	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X
	D3DXCOLOR col;					//�F
	int nCounterAnim;				//�A�j���[�V�����J�E���^�[
	int nPatternAnim;				//�A�j���[�V�����p�^�[��No.
	bool bUse;						//�g�p���Ă��邩�ǂ���
}Explosion;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;			//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;		//���_�o�b�t�@�̃|�C���^
Explosion g_aExplosion[MAX_EXPLOSION];					//�����̏��

//====================================================================
//�����̏���������
//====================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�ւ̃|�C���^
	int nCntExplosion;

	//�f�o�C�X�̏���
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\magic001.png",
		&g_pTextureExplosion);

	//�����̏��̏�����
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_3D*pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//���_���W�̐ݒ� 
		pVtx[0].pos = D3DXVECTOR3(-5.0f, +5.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+5.0f, +5.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-5.0f, -5.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+5.0f, -5.0f, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.0f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);
		pVtx[1].tex = D3DXVECTOR2(0.2f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.0f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.2f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);
		pVtx[3].tex = D3DXVECTOR2(0.2f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.2f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//====================================================================
//�����̏I������
//====================================================================
void UninitExplosion(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//====================================================================
//�����̍X�V����
//====================================================================
void UpdateExplosion(void)
{
	int nCntExplosion;

	VERTEX_3D*pVtx;	//���_�|�C���^������

					//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//�������g�p����Ă���
			g_aExplosion[nCntExplosion].nCounterAnim++;	//�J�E���^�[�����Z
			if (g_aExplosion[nCntExplosion].nCounterAnim % 3 == 0)
			{
				//�p�^�[��No.���X�V����
				g_aExplosion[nCntExplosion].nPatternAnim++;

				//���_���W�̐ݒ� 
				pVtx[0].pos = D3DXVECTOR3(-5.0f, +5.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(+5.0f, +5.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-5.0f, -5.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(+5.0f, -5.0f, 0.0f);

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.0f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.0f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.2f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + (g_aExplosion[nCntExplosion].nPatternAnim % 5)* 0.2f, 0.2f + (g_aExplosion[nCntExplosion].nPatternAnim / 5)* 0.2f);

				if (g_aExplosion[nCntExplosion].nPatternAnim > 25)
				{
					g_aExplosion[nCntExplosion].bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���
				}

			}
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//====================================================================
//�����̕`�揈��
//====================================================================
void DrawExplosion(void)
{
	int nCntEx;

	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;	//�r���[�}�g���b�N�X�����p

						//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (nCntEx = 0; nCntEx < MAX_EXPLOSION; nCntEx++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aExplosion[nCntEx].mtxWorld);

		//�r���[�}�g���b�N�X������
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//�|���S�����J�����ɑ΂��Đ��ʂ�������
		D3DXMatrixInverse(&g_aExplosion[nCntEx].mtxWorld, NULL, &mtxView);	//�t�s������߂�
		g_aExplosion[nCntEx].mtxWorld._41 = 0.0f;
		g_aExplosion[nCntEx].mtxWorld._42 = 0.0f;
		g_aExplosion[nCntEx].mtxWorld._43 = 0.0f;

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntEx].pos.x, g_aExplosion[nCntEx].pos.y, g_aExplosion[nCntEx].pos.z);

		D3DXMatrixMultiply(&g_aExplosion[nCntEx].mtxWorld, &g_aExplosion[nCntEx].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntEx].mtxWorld);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureExplosion);

		if (g_aExplosion[nCntEx].bUse == true)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				4 * nCntEx,										//�v���~�e�B�u(�|���S��)��
				2);
		}
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//====================================================================
//�����̐ݒ菈��
//====================================================================
void SetExplosion(D3DXVECTOR3 pos, int nType)
{
	int nCntExplosion;

	for (nCntExplosion = 0; nCntExplosion <MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//�������g�p����Ă��Ȃ�
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;	//�������g�p���Ă����Ԃɂ���

			break;
		}
	}
}