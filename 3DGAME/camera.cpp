#include "main.h"
#include "camera.h"
#include "model.h"
#include "input.h"

#define CAMERA_DISTANCE (300.0f)	//���_�ƒ����_�̋���
#define MODEL_DISTANCE (10.0f)	//���f���ƒ����_�̋���
#define CAMERA_SPEED (3.0f)	//�J�����̈ړ��X�s�[�h
#define CAMERA_VR_SPEED (0.03f)	//�J�����̎��_�X�s�[�h
#define CAMERA_HOMING (0.1f)	//�J�����̒Ǐ]�X�s�[�h

//�O���[�o���ϐ�
Camera g_camera;	//�J�����̏��

//====================================================================
//�J�����̏���������
//====================================================================
void InitCamera(void)
{
	g_camera.posV = D3DXVECTOR3(0.0f, 50.0f, 50.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.rot = D3DXVECTOR3(-0.5f, 0.0f, 0.0f);
	g_camera.rotOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.Mode = CAMERA_MODE_3;
}

//====================================================================
//�J�����̏I������
//====================================================================
void UninitCamera(void)
{

}

//====================================================================
//�J�����̍X�V����
//====================================================================
void UpdateCamera(void)
{
	//���_�؂�ւ�
	if (GetKeyboardTrigger(DIK_F3) == true || GetGamepadTrigger(BUTTON_R_PUSH, 0) == true)
	{//F3�܂���R�X�e�B�b�N�������݂Ŏ��_�̃��[�h��؂�ւ���
		g_camera.Mode = (g_camera.Mode == CAMERA_MODE_1) ? CAMERA_MODE_3 : CAMERA_MODE_1;

		switch (g_camera.Mode)
		{
		case CAMERA_MODE_1:	//��l�̎��_

			g_camera.posV = g_camera.posR;

			break;

		case CAMERA_MODE_3:	//�O�l�̎��_

			g_camera.posR = g_camera.posV;

			break;
		}
	}

	if (g_camera.rot.x <= D3DX_PI * 0.5f && g_camera.rot.x >= -(D3DX_PI * 0.5f))
	{//����
		g_camera.rotOld.x = g_camera.rot.x;

		//�L�[�{�[�h
		if (GetKeyboardPress(DIK_I) == true)
		{
			g_camera.rot.x += CAMERA_VR_SPEED;
		}
		if (GetKeyboardPress(DIK_K) == true)
		{
			g_camera.rot.x -= CAMERA_VR_SPEED;
		}

		//�E�X�e�B�b�N�̏㉺���_�ړ�����
		g_camera.rot.x += GetGamepad_Stick_Right(0).y * CAMERA_VR_SPEED;

		if (GetMousePress(PUSH_LEFT) == true || GetMousePress(PUSH_RIGHT) == true)
		{
			g_camera.rot.x -= GetMouseMove().y * CAMERA_VR_SPEED;
		}
	}

	//�E�X�e�B�b�N�̏㉺���_�ړ�����
	if (fabsf(g_camera.rot.x) > fabsf(D3DX_PI * 0.5f))
	{//����ɒB�������P�t���[���O��rot�ɂ��ǂ�
		g_camera.rot.x = g_camera.rotOld.x;
	}

	//�L�[�{�[�h
	if (GetKeyboardPress(DIK_J) == true)
	{
		g_camera.rot.y -= CAMERA_VR_SPEED;

	}
	if (GetKeyboardPress(DIK_L) == true)
	{
		g_camera.rot.y += CAMERA_VR_SPEED;
	}

	//�E�X�e�B�b�N�̍��E���_�ړ�����
	g_camera.rot.y += GetGamepad_Stick_Right(0).x * CAMERA_VR_SPEED;

	if (GetMousePress(PUSH_LEFT) == true || GetMousePress(PUSH_RIGHT) == true)
	{
		g_camera.rot.y += GetMouseMove().x * CAMERA_VR_SPEED;
	}

	//����������̌����̕␳
	if (g_camera.rot.y > D3DX_PI * 1.0f)
	{
		g_camera.rot.y -= D3DX_PI * 2.0f;
	}
	else if (g_camera.rot.y < -D3DX_PI * 1.0f)
	{
		g_camera.rot.y += D3DX_PI * 2.0f;
	}


	//�L�[�{�[�h
	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
		g_camera.posV.y += CAMERA_SPEED;
		g_camera.posR.y += CAMERA_SPEED;

	}
	if (GetKeyboardPress(DIK_LCONTROL) == true)
	{
		g_camera.posV.y -= CAMERA_SPEED;
		g_camera.posR.y -= CAMERA_SPEED;
	}
	if (GetKeyboardPress(DIK_RSHIFT) == true)
	{
		g_camera.posV.y += CAMERA_SPEED;
		g_camera.posR.y += CAMERA_SPEED;

	}
	if (GetKeyboardPress(DIK_RCONTROL) == true)
	{
		g_camera.posV.y -= CAMERA_SPEED;
		g_camera.posR.y -= CAMERA_SPEED;
	}

	//���g���K�[���������ނƃJ������������
	g_camera.posV.y -= GetGamepad_Trigger_Left(0);
	g_camera.posR.y -= GetGamepad_Trigger_Left(0);

	//�E�g���K�[���������ނƃJ�������オ��
	g_camera.posV.y += GetGamepad_Trigger_Right(0);
	g_camera.posR.y += GetGamepad_Trigger_Right(0);

	//�L�[�{�[�h
	if (GetKeyboardPress(DIK_W) == true)
	{
		g_camera.posR.z += CAMERA_SPEED * cosf(g_camera.rot.y);
		g_camera.posR.x += CAMERA_SPEED * sinf(g_camera.rot.y);

		g_camera.posV.z += CAMERA_SPEED * cosf(g_camera.rot.y);
		g_camera.posV.x += CAMERA_SPEED * sinf(g_camera.rot.y);

	}
	if (GetKeyboardPress(DIK_S) == true)
	{
		g_camera.posR.z += -CAMERA_SPEED * cosf(g_camera.rot.y);
		g_camera.posR.x += -CAMERA_SPEED * sinf(g_camera.rot.y);

		g_camera.posV.z += -CAMERA_SPEED * cosf(g_camera.rot.y);
		g_camera.posV.x += -CAMERA_SPEED * sinf(g_camera.rot.y);
	}
	if (GetKeyboardPress(DIK_A) == true)
	{
		g_camera.posR.x += -CAMERA_SPEED * cosf(g_camera.rot.y);
		g_camera.posR.z -= -CAMERA_SPEED * sinf(g_camera.rot.y);

		g_camera.posV.x += -CAMERA_SPEED * cosf(g_camera.rot.y);
		g_camera.posV.z -= -CAMERA_SPEED * sinf(g_camera.rot.y);
	}
	if (GetKeyboardPress(DIK_D) == true)
	{
		g_camera.posR.x += CAMERA_SPEED * cosf(g_camera.rot.y);
		g_camera.posR.z -= CAMERA_SPEED * sinf(g_camera.rot.y);

		g_camera.posV.x += CAMERA_SPEED * cosf(g_camera.rot.y);
		g_camera.posV.z -= CAMERA_SPEED * sinf(g_camera.rot.y);
	}

	//���X�e�B�b�N�ɂ��O��ړ�	
	g_camera.posV.z += GetGamepad_Stick_Left(0).y * cosf(g_camera.rot.y);
	g_camera.posV.x += GetGamepad_Stick_Left(0).y * sinf(g_camera.rot.y);

	//���X�e�B�b�N�ɂ�鍶�E�ړ�
	g_camera.posV.x += GetGamepad_Stick_Left(0).x * cosf(g_camera.rot.y);
	g_camera.posV.z -= GetGamepad_Stick_Left(0).x * sinf(g_camera.rot.y);

	//���X�e�B�b�N�ɂ��O��ړ�	
	g_camera.posR.z += GetGamepad_Stick_Left(0).y * cosf(g_camera.rot.y);
	g_camera.posR.x += GetGamepad_Stick_Left(0).y * sinf(g_camera.rot.y);

	//���X�e�B�b�N�ɂ�鍶�E�ړ�
	g_camera.posR.x += GetGamepad_Stick_Left(0).x * cosf(g_camera.rot.y);
	g_camera.posR.z -= GetGamepad_Stick_Left(0).x * sinf(g_camera.rot.y);

	//���f���Ǐ]����
	Model * pModel = GetModel();
	g_camera.posRDest.x = g_camera.posV.x + sinf(g_camera.rot.y) * cosf(g_camera.rot.x) * CAMERA_DISTANCE;
	g_camera.posRDest.z = g_camera.posV.z + cosf(g_camera.rot.y) * cosf(g_camera.rot.x) * CAMERA_DISTANCE;
	g_camera.posRDest.y = g_camera.posV.y + sinf(g_camera.rot.x) * CAMERA_DISTANCE;

	g_camera.posVDest.x = pModel->pos.x + sinf(pModel->rot.y) * MODEL_DISTANCE + sinf(g_camera.rot.y) * -cosf(g_camera.rot.x) * CAMERA_DISTANCE;
	g_camera.posVDest.z = pModel->pos.z + cosf(pModel->rot.y) * MODEL_DISTANCE + cosf(g_camera.rot.y) * -cosf(g_camera.rot.x) * CAMERA_DISTANCE;
	g_camera.posVDest.y = pModel->pos.y + sinf(-g_camera.rot.x) * CAMERA_DISTANCE;
	
	if (fabsf(pModel->move.x) <= 0.5f && fabsf(pModel->move.z) <= 0.5f)
	{
		g_camera.posR += (g_camera.posRDest - g_camera.posR) * 0.05f;
		g_camera.posV += (g_camera.posVDest - g_camera.posV) * 0.1f;
	}
	else
	{
		g_camera.posR += (g_camera.posRDest - g_camera.posR) * CAMERA_HOMING;
		g_camera.posV += (g_camera.posVDest - g_camera.posV) * CAMERA_HOMING;
	}

	////���f�����~�܂������ɐ��ʂ���������
	//float fRotMove, fRotDest, fRotDiff;

	//if (pModel->move.x == 0.0f && pModel->move.z == 0.0f)
	//{
	//	fRotMove = atan2f(sinf(g_camera.rot.y), cosf(g_camera.rot.y));	//���݂̌���
	//	fRotDest = atan2f(sinf(pModel->rot.y), cosf(pModel->rot.y));	//�ړI�̌���
	//	fRotDiff = fRotDest - fRotMove;									 //����

	//	if (fRotDiff > D3DX_PI * 1.0f)
	//	{
	//		fRotDiff -= D3DX_PI * 2.0f;
	//	}
	//	else if (fRotDiff < -D3DX_PI * 1.0f)
	//	{
	//		fRotDiff += D3DX_PI * 2.0f;
	//	}

	//	fRotMove += fRotDiff * 0.1f;

	//	if (fRotMove > D3DX_PI * 1.0f)
	//	{
	//		fRotMove -= D3DX_PI * 2.0f;
	//	}
	//	else if (fRotMove < -D3DX_PI * 1.0f)
	//	{
	//		fRotMove += D3DX_PI * 2.0f;
	//	}
	//	g_camera.rot.y = fRotMove;
	//}


	//�}�E�X����
	if (GetMousePress(PUSH_LEFT) == true || GetMousePress(PUSH_RIGHT) == true)
	{
		if (GetMousePress(PUSH_LEFT) == true && GetMousePress(PUSH_RIGHT) == true)
		{

		}
		if (GetMousePress(PUSH_RIGHT) == true)
		{
			//���_�̏����o�͂���
			g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * cosf(g_camera.rot.x) * CAMERA_DISTANCE;
			g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * cosf(g_camera.rot.x) * CAMERA_DISTANCE;
			g_camera.posR.y = g_camera.posV.y + sinf(g_camera.rot.x) * CAMERA_DISTANCE;
		}
		if (GetMousePress(PUSH_LEFT) == true)
		{
			//���_�̏����o�͂���
			g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y) * -cosf(g_camera.rot.x) * CAMERA_DISTANCE;
			g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y) * -cosf(g_camera.rot.x) * CAMERA_DISTANCE;
			g_camera.posV.y = g_camera.posR.y + sinf(-g_camera.rot.x) * CAMERA_DISTANCE;
		}
	}
	//else
	//{//�}�E�X���삪����Ă��Ȃ��ꍇ
	//	switch (g_camera.Mode)
	//	{
	//	case CAMERA_MODE_1:	//��l�̎��_

	//		//�����_�̏����o�͂���
	//		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * cosf(g_camera.rot2.z) * CAMERA_DISTANCE;
	//		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * cosf(g_camera.rot2.x) * CAMERA_DISTANCE;
	//		g_camera.posR.y = g_camera.posV.y + sinf(g_camera.rot2.y) * CAMERA_DISTANCE;

	//		break;

	//	case CAMERA_MODE_3:	//�O�l�̎��_

	//		//���_�̏����o�͂���
	//		g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y) * -cosf(g_camera.rot2.z) * CAMERA_DISTANCE;
	//		g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y) * -cosf(g_camera.rot2.x) * CAMERA_DISTANCE;
	//		g_camera.posV.y = g_camera.posR.y + sinf(-g_camera.rot2.y) * CAMERA_DISTANCE;

	//		break;
	//	}
	//}
}

//====================================================================
//�J�����̐ݒ菈��
//====================================================================
void SetCamera(void)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		1000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	//�r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

D3DXVECTOR3 GetposV(void)
{
	return g_camera.posV;
}
D3DXVECTOR3 GetposR(void)
{
	return g_camera.posR;
}
D3DXVECTOR3 GetvecU(void)
{
	return g_camera.vecU;
}
D3DXVECTOR3 Getrot(void)
{
	return g_camera.rot;
}
CAMERA_MODE GetCameraMode(void)
{
	return g_camera.Mode;
}