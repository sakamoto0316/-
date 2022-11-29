#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//�u���b�N�̎��
typedef enum
{
	CAMERA_MODE_1 = 0,		//��l�̎��_
	CAMERA_MODE_3,			//�O�l�̎��_
	CAMERA_MODE_MAX
}CAMERA_MODE;

//�J�����̍\���̂��`
typedef struct
{
	D3DXVECTOR3 posV;			//���_
	D3DXVECTOR3 posR;			//�����_
	D3DXVECTOR3 vecU;			//������x�N�g��
	D3DXMATRIX mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			//�r���[�}�g���b�N�X
	D3DXVECTOR3 rot;
	D3DXVECTOR3 rotOld;
	D3DXVECTOR3 posVDest;		//�ړI�̎��_
	D3DXVECTOR3 posRDest;		//�ړI�̒����_
	CAMERA_MODE Mode;
}Camera;

//�v���g�^�C�v�錾
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
D3DXVECTOR3 GetposV(void);
D3DXVECTOR3 GetposR(void);
D3DXVECTOR3 GetvecU(void);
D3DXVECTOR3 Getrot(void);
CAMERA_MODE GetCameraMode(void);
#endif