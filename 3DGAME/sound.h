#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
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
	//SOUND_LABEL_SE_ATTACK_SKY,			// 攻撃
	//SOUND_LABEL_SE_ATTACK_NORMAL,		// 攻撃
	//SOUND_LABEL_SE_ATTACK_CROUCHING,	// 攻撃
	//SOUND_LABEL_SE_DAMAGE_BOSS,			// ダメージ
	//SOUND_LABEL_SE_DAMAGE_PLAYER,		// ダメージ
	//SOUND_LABEL_SE_MOVE,				// 移動
	//SOUND_LABEL_SE_ENTER,				// 決定
	//SOUND_LABEL_SE_BOSSATTACK_WALL,		// ボスの攻撃
	//SOUND_LABEL_SE_BOSSATTACK_READY,	// ボスの攻撃
	//SOUND_LABEL_SE_BOSSATTACK_GO,		// ボスの攻撃
	//SOUND_LABEL_SE_BOSSATTACK_SHOT,		// ボスの攻撃
	//SOUND_LABEL_SE_BOSSDEAD,			// ボスの死亡
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
