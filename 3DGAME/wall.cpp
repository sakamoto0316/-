#include "main.h"
#include "wall.h"
#include "billboard.h"
#include "shadow.h"
#include "explosion.h"
#include "particle.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureWall;				//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall;			//���_�o�b�t�@�̃|�C���^
Wall g_Wall[MAX_WALL];							//�ǂ̏��
int g_nIndexWall[MAX_WALL];
//====================================================================
//�ǂ̏���������
//====================================================================
void InitWall(void)
{
	int nCntWall;

	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ENEMY00.png",
		&g_pTextureWall);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		switch (nCntWall)
		{
		case 0:
			g_Wall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.25f, 0.0f);
			break;

		case 1:
			g_Wall[nCntWall].pos = D3DXVECTOR3(50.0f, 0.0f, 200.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			break;

		case 2:
			g_Wall[nCntWall].pos = D3DXVECTOR3(-50.0f, 0.0f, -200.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
			break;

		case 3:
			g_Wall[nCntWall].pos = D3DXVECTOR3(50.0f, 0.0f, -200.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
			break;

		case 4:
			g_Wall[nCntWall].pos = D3DXVECTOR3(200.0f, 0.0f, -50.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
			break;

		case 5:
			g_Wall[nCntWall].pos = D3DXVECTOR3(200.0f, 0.0f, 50.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
			break;

		case 6:
			g_Wall[nCntWall].pos = D3DXVECTOR3(-200.0f, 0.0f, -50.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
			break;

		case 7:
			g_Wall[nCntWall].pos = D3DXVECTOR3(-200.0f, 0.0f, 50.0f);
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
			break;
		}
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D*pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//���_���W�̐ݒ� 
		pVtx[0].pos = D3DXVECTOR3(-MAX_WIDTH, MAX_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+MAX_WIDTH, MAX_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-MAX_WIDTH, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+MAX_WIDTH, 0.0f, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//�e�̐ݒ�
		g_nIndexWall[nCntWall] = SetShadow();

		//�e�̈ʒu��ݒ�
		SetPositionShadow(g_nIndexWall[nCntWall], g_Wall[nCntWall].pos);
	}
}

//====================================================================
//�ǂ̏I������
//====================================================================
void UninitWall(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//====================================================================
//�ǂ̍X�V����
//====================================================================
void UpdateWall(void)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		CollisionBillboard(&g_Wall[nCntWall]);
	}
	//g_Wall[0].rot.y += 0.001f;
}

//====================================================================
//�ǂ̕`�揈��
//====================================================================
void DrawWall(void)
{
	int nCntWall;

	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Wall[nCntWall].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);

		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);

		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCntWall].mtxWorld);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureWall);

		//�ǂ̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			4 * nCntWall,										//�v���~�e�B�u(�|���S��)��
			2);
	}
}

//====================================================================
//�ǂƂ̓����蔻�菈��
//====================================================================
void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{

		//if (g_Wall[nCntWall].rot.y == 0.0f || g_Wall[nCntWall].rot.y == D3DX_PI)
		//{//Z���ɑ΂��Đ��ʂ̕�
		//	if (
		//		(pPos->z + max.z >= g_Wall[nCntWall].pos.z && pPosOld->z + max.z< g_Wall[nCntWall].pos.z ||
		//		pPos->z + min.z <= g_Wall[nCntWall].pos.z && pPosOld->z + min.z > g_Wall[nCntWall].pos.z) &&
		//		pPos->x + max.x >= g_Wall[nCntWall].pos.x - MAX_WIDTH &&
		//		pPos->x + min.x <= g_Wall[nCntWall].pos.x + MAX_WIDTH &&
		//		pPos->y + max.y >= g_Wall[nCntWall].pos.y &&
		//		pPos->y + min.y <= g_Wall[nCntWall].pos.y + MAX_HEIGHT
		//		)
		//	{//�ǂƃv���C���[����������
		//		pPos->z = pPosOld->z;
		//	}
		//}

		//if (g_Wall[nCntWall].rot.y == D3DX_PI * 0.5f || g_Wall[nCntWall].rot.y == D3DX_PI * -0.5f)
		//{//X���ɑ΂��Đ��ʂ̕�
		//	if (
		//		(pPos->x + max.x >= g_Wall[nCntWall].pos.x && pPosOld->x + max.x < g_Wall[nCntWall].pos.x ||
		//		pPos->x + min.x <= g_Wall[nCntWall].pos.x && pPosOld->x + min.x > g_Wall[nCntWall].pos.x) &&
		//		pPos->z + max.z >= g_Wall[nCntWall].pos.z - MAX_WIDTH &&
		//		pPos->z + min.z <= g_Wall[nCntWall].pos.z + MAX_WIDTH &&
		//		pPos->y + max.y >= g_Wall[nCntWall].pos.y &&
		//		pPos->y + min.y <= g_Wall[nCntWall].pos.y + MAX_HEIGHT
		//		)
		//	{//�ǂƃv���C���[����������
		//		pPos->x = pPosOld->x;
		//	}
		//}
	}
}

//====================================================================
//�e�Ƃ̓����蔻�菈��
//====================================================================
void CollisionBillboard(Wall*pWall)
{
	Billboard * pBillboard;		//�e�̏��ւ̃|�C���^

	D3DXVECTOR3 vecToPos0;
	D3DXVECTOR3 vecToPos1;
	D3DXVECTOR3 vecToPos3;
	D3DXVECTOR3 vecToPos2;

	D3DXVECTOR3 vecToPosOld0;
	D3DXVECTOR3 vecToPosOld1;
	D3DXVECTOR3 vecToPosOld3;
	D3DXVECTOR3 vecToPosOld2;

	D3DXVECTOR3 vecLine01;
	D3DXVECTOR3 vecLine13;
	D3DXVECTOR3 vecLine32;
	D3DXVECTOR3 vecLine20;

	//�G�̏���
	pBillboard = GetBillboard();		//�G�̏��̐擪�A�h���X����������

	VERTEX_3D*pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pBillboard++)
	{
		if (pBillboard->bUse == true)
		{
			vecToPos0.x = (sinf(pWall->rot.y) * (pWall->pos.x + pVtx[0].pos.x));
			vecToPos0.y = 0.0f;
			vecToPos0.z = pBillboard->pos.z - (cosf(pWall->rot.y) * (pWall->pos.z + pVtx[0].pos.z));

			//vecToPosOld0 = pBillboard->posOld - (pWall->pos + pVtx[0].pos);

			//100
			vecLine01.x = cosf(pWall->rot.y) * (pVtx[1].pos.x - pVtx[0].pos.x);
			vecLine01.y = 0.0f;
			vecLine01.z = sinf(pWall->rot.y) * (pVtx[1].pos.x - pVtx[0].pos.x);


			vecToPos3 = pBillboard->pos - (pWall->pos + pVtx[3].pos);
			vecToPosOld3 = pBillboard->posOld - (pWall->pos + pVtx[3].pos);
			vecLine32 = pWall->pos + pVtx[2].pos - (pWall->pos + pVtx[3].pos);

			if (((vecLine01.z * vecToPos0.x) - (vecLine01.x * vecToPos0.z)) < 0 /*&& ((vecLine01.z * vecToPosOld0.x) - (vecLine01.x * vecToPosOld0.z)) > 0 ||
				((vecLine32.z * vecToPos3.x) - (vecLine32.x * vecToPos3.z)) > 0 && ((vecLine32.z * vecToPosOld3.x) - (vecLine32.x * vecToPosOld3.z)) < 0*/)
			{
					pBillboard->bUse = false;
					falseShadow(pBillboard->nIndex);
					SetExplosion(pBillboard->pos, 0);
					SetParticle(pBillboard->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 60, 5.0f, 4);
			}
		}
		//pVtx += 4;	//���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}
//====================================================================
//�ǂ̏���
//====================================================================
Wall * GetWall(void)
{
	return &g_Wall[0];
}