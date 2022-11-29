#include "main.h"
#include "light.h"

#define MAX_LIGHT (3)	//�g�p���郉�C�g�̐�

//�O���[�o���ϐ�
D3DLIGHT9 g_light[MAX_LIGHT];	//���C�g�̏��

//====================================================================
//�|���S���̏���������
//====================================================================
void InitLight(void)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir[MAX_LIGHT];	//�ݒ�p�����x�N�g��

									//���C�g�̏����N���A����
	ZeroMemory(&g_light, sizeof(D3DLIGHT9));

	for (int nCntlight = 0; nCntlight < MAX_LIGHT; nCntlight++)
	{
		//���C�g�̎�ނ�ݒ�
		g_light[nCntlight].Type = D3DLIGHT_DIRECTIONAL;

		//���C�g�̊g�U����ݒ�
		g_light[nCntlight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//���C�g�̕�����ݒ�
		switch (nCntlight)
		{
		case 0:
			vecDir[nCntlight] = D3DXVECTOR3(0.2f, -0.8f, -0.5f);
			break;
		case 1:
			vecDir[nCntlight] = D3DXVECTOR3(-0.5f, -0.8f, -0.5f);
			break;
		case 2:
			vecDir[nCntlight] = D3DXVECTOR3(0.0f, -0.8f, 0.6f);
			break;
		}
		D3DXVec3Normalize(&vecDir[nCntlight], &vecDir[nCntlight]);	//�x�N�g���𐳋K������
		g_light[nCntlight].Direction = vecDir[nCntlight];

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntlight, &g_light[nCntlight]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntlight, TRUE);
	}
}

//====================================================================
//�|���S���̏I������
//====================================================================
void UninitLight(void)
{

}

//====================================================================
//�|���S���̍X�V����
//====================================================================
void UpdateLight(void)
{

}