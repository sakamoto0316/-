#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,			// BGM0
	//SOUND_LABEL_BGM_TUTORIAL,			// BGM1
	//SOUND_LABEL_BGM_BOSS_E,				// BGM2
	//SOUND_LABEL_BGM_BOSS_N,				// BGM3
	//SOUND_LABEL_BGM_BOSS_H,				// BGM4
	//SOUND_LABEL_BGM_BOSS_VH,			// BGM5
	//SOUND_LABEL_BGM_RESULT_CLEAR,		// BGM6
	//SOUND_LABEL_BGM_RESULT_OVER,		// BGM7
	//SOUND_LABEL_SE_ATTACK_SKY,			// �U��
	//SOUND_LABEL_SE_ATTACK_NORMAL,		// �U��
	//SOUND_LABEL_SE_ATTACK_CROUCHING,	// �U��
	//SOUND_LABEL_SE_DAMAGE_BOSS,			// �_���[�W
	//SOUND_LABEL_SE_DAMAGE_PLAYER,		// �_���[�W
	//SOUND_LABEL_SE_MOVE,				// �ړ�
	//SOUND_LABEL_SE_ENTER,				// ����
	//SOUND_LABEL_SE_BOSSATTACK_WALL,		// �{�X�̍U��
	//SOUND_LABEL_SE_BOSSATTACK_READY,	// �{�X�̍U��
	//SOUND_LABEL_SE_BOSSATTACK_GO,		// �{�X�̍U��
	//SOUND_LABEL_SE_BOSSATTACK_SHOT,		// �{�X�̍U��
	//SOUND_LABEL_SE_BOSSDEAD,			// �{�X�̎��S
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
