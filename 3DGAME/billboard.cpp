#include "main.h"
#include "billboard.h"
#include "shadow.h"
#include "effect.h"
#include "explosion.h"
#include "particle.h"
#include "wall.h"

#define BILLBOARD_LIFE (200)	//�e�̎���
#define BILLBOARD_SIZE (5.0f)	//�e�̑傫��

//�v���g�^�C�v�錾
void CollisionWall(Billboard*pBillboard, int nCnt);

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBillboard;				//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard;		//���_�o�b�t�@�̃|�C���^
Billboard g_aBillboard[MAX_BILLBOARD];

//====================================================================
//�r���{�[�h�̏���������
//====================================================================
void InitBillboard(void)
{
	int nCntBill;
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BOSS00.png",
		&g_pTextureBillboard);

	for (nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		g_aBillboard[nCntBill].pos = D3DXVECTOR3(90.0f, 10.0f, 50.0f);
		g_aBillboard[nCntBill].nIndex = -1;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D*pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		//���_���W�̐ݒ� 
		pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE, +BILLBOARD_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+BILLBOARD_SIZE, +BILLBOARD_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}

//====================================================================
//�r���{�[�h�̏I������
//====================================================================
void UninitBillboard(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//====================================================================
//�r���{�[�h�̍X�V����
//====================================================================
void UpdateBillboard(void)
{
	int nCntBill;

	for (nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == true)
		{
			g_aBillboard[nCntBill].posOld = g_aBillboard[nCntBill].pos;

			//�e�̈ʒu��ݒ�
			SetPositionShadow(g_aBillboard[nCntBill].nIndex, g_aBillboard[nCntBill].pos);

			//�e�̔��ˌ������w�肷��
			g_aBillboard[nCntBill].pos.z += cosf(g_aBillboard[nCntBill].rot);
			g_aBillboard[nCntBill].pos.x += sinf(g_aBillboard[nCntBill].rot);
			g_aBillboard[nCntBill].nLife--;

			//�ǂƂ̓����蔻��
			CollisionWall(&g_aBillboard[nCntBill], nCntBill);
			Wall *pWall = GetWall();


			//�e���ˎ��ɃG�t�F�N�g���Ăяo��
			SetEffect(g_aBillboard[nCntBill].pos,D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),8.0f,30);


			//�������Ȃ��Ȃ������e������
			if (g_aBillboard[nCntBill].nLife <= 0)
			{
				g_aBillboard[nCntBill].bUse = false;
				falseShadow(g_aBillboard[nCntBill].nIndex);
				SetExplosion(g_aBillboard[nCntBill].pos, 0);
			}
		}
	}

	VERTEX_3D*pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == true)
		{
			//���_���W�̐ݒ� 
			pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE, +BILLBOARD_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+BILLBOARD_SIZE, +BILLBOARD_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f);
		}

		pVtx += 4;	//���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}

//====================================================================
//�r���{�[�h�̕`�揈��
//====================================================================
void DrawBillboard(void)
{
	int nCntBill;

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

	for (nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aBillboard[nCntBill].mtxWorld);

		//�r���[�}�g���b�N�X������
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//�|���S�����J�����ɑ΂��Đ��ʂ�������
		D3DXMatrixInverse(&g_aBillboard[nCntBill].mtxWorld, NULL, &mtxView);	//�t�s������߂�
		g_aBillboard[nCntBill].mtxWorld._41 = 0.0f;
		g_aBillboard[nCntBill].mtxWorld._42 = 0.0f;
		g_aBillboard[nCntBill].mtxWorld._43 = 0.0f;

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBill].pos.x, g_aBillboard[nCntBill].pos.y, g_aBillboard[nCntBill].pos.z);

		D3DXMatrixMultiply(&g_aBillboard[nCntBill].mtxWorld, &g_aBillboard[nCntBill].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBill].mtxWorld);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBillboard);

		if (g_aBillboard[nCntBill].bUse == true)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				4 * nCntBill,										//�v���~�e�B�u(�|���S��)��
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
//�r���{�[�h�̐ݒ菈��
//====================================================================
void SetBillboard(D3DXVECTOR3 pos, float rot)
{
	int nCntBill;

	VERTEX_2D*pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == false)
		{
			g_aBillboard[nCntBill].pos = pos;
			g_aBillboard[nCntBill].rot = rot;
			g_aBillboard[nCntBill].nLife = BILLBOARD_LIFE;
			g_aBillboard[nCntBill].bUse = true;

			//�e�̐ݒ�
			g_aBillboard[nCntBill].nIndex = SetShadow();
			break;
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}

//====================================================================
//�e�ƕǂ̓����蔻��
//====================================================================
void CollisionWall(Billboard*pBillboard,int nCnt)
{
	//Wall * pWall;		//�G�̏��ւ̃|�C���^
	//int nCntWall;

	//D3DXVECTOR3 vecToPos;
	//D3DXVECTOR3 vecLine;

	////�G�̏���
	//pWall = GetWall();		//�G�̏��̐擪�A�h���X����������

	//VERTEX_3D*pVtx;	//���_�|�C���^������

	////���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	//g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	//{
	//	vecToPos = pBillboard->pos - D3DXVECTOR3(pWall->pos.x - 50, pWall->pos.y, pWall->pos.z);
	//	vecLine = D3DXVECTOR3(pWall->pos.x - 50, pWall->pos.y, pWall->pos.z) - D3DXVECTOR3(pWall->pos.x + 50, pWall->pos.y, pWall->pos.z);

	//	if (((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z)) > 0)
	//	{
	//		pBillboard->bUse = false;
	//		falseShadow(pBillboard->nIndex);
	//		SetExplosion(pBillboard->pos, 0);
	//		SetParticle(pBillboard->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 60, 5.0f, 4);
	//	}

	//	pVtx += 4;	//���_�f�[�^�̃|�C���^���S���i�߂�
	//}

	////���_�o�b�t�@���A�����b�N����
	//g_pVtxBuffBillboard->Unlock();

	//for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	//{
	//	if (pWall->rot.y == 0.0f || pWall->rot.y == D3DX_PI)
	//	{
	//		if (
	//			(pBillboard->pos.z >= pWall->pos.z && pBillboard->posOld.z < pWall->pos.z ||
	//			pBillboard->pos.z <= pWall->pos.z && pBillboard->posOld.z > pWall->pos.z) &&
	//			pBillboard->pos.x >= pWall->pos.x - MAX_WIDTH &&
	//			pBillboard->pos.x <= pWall->pos.x + MAX_WIDTH &&
	//			pBillboard->pos.y >= pWall->pos.y &&
	//			pBillboard->pos.y <= pWall->pos.y + MAX_HEIGHT
	//			)
	//		{//�G�ƍU������������
	//			pBillboard->bUse = false;
	//			falseShadow(g_nIndexBillboard[nCnt]);
	//			SetExplosion(pBillboard->pos, 0);
	//			SetParticle(pBillboard->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 60, 5.0f, 4);
	//		}
	//	}

	//	if (pWall->rot.y == D3DX_PI * 0.5f || pWall->rot.y == D3DX_PI * -0.5f)
	//	{
	//		if (
	//			(pBillboard->pos.x >= pWall->pos.x && pBillboard->posOld.x < pWall->pos.x ||
	//			pBillboard->pos.x <= pWall->pos.x && pBillboard->posOld.x > pWall->pos.x) &&
	//			pBillboard->pos.z >= pWall->pos.z - MAX_WIDTH &&
	//			pBillboard->pos.z <= pWall->pos.z + MAX_WIDTH &&
	//			pBillboard->pos.y >= pWall->pos.y &&
	//			pBillboard->pos.y <= pWall->pos.y + MAX_HEIGHT
	//			)
	//		{//�G�ƍU������������
	//			pBillboard->bUse = false;
	//			falseShadow(g_nIndexBillboard[nCnt]);
	//			SetExplosion(pBillboard->pos, 0);
	//			SetParticle(pBillboard->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),60,5.0f,4);
	//		}
	//	}
	//}
}

//====================================================================
//�e�̏���
//====================================================================
Billboard * GetBillboard(void)
{
	return &g_aBillboard[0];
}