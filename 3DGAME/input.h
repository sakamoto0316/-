#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "Xinput.h"

#pragma comment(lib,"xinput.lib")	//入力処理

//列挙型定義
typedef enum
{
	BUTTON_UP = 0,	//十字キーの上ボタン
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_START,
	BUTTON_BACK,
	BUTTON_L_PUSH,
	BUTTON_R_PUSH,
	BUTTON_L,
	BUTTON_R,
	BUTTON_NAZO1,
	BUTTON_NAZO2,
	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_MAX
}JOYKEY;

typedef enum
{
	PUSH_LEFT = 0,
	PUSH_RIGHT,
	PUSH_WHEEL,
	PUSH_SIDE_1,
	PUSH_SIDE_2,
	PUSH_MAX,
}MOUSE_PUSH;

//プロトタイプ宣言
HRESULT InitDevice(HINSTANCE hInstance, HWND hWnd);
void UninitDevice(void);
void UpdateDevice(void);

//キーボード用
HRESULT InitKeyboard(HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);

bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);

//ゲームパッド用
HRESULT InitGamepad(HWND hWnd);
void UninitGamepad(void);
void UpdateGamepad(void);

bool GetGamepadPress(JOYKEY key, int nPlayer);
bool GetGamepadTrigger(JOYKEY key, int nPlayer);
bool GetGamepadRelease(JOYKEY key, int nPlayer);
D3DXVECTOR3 GetGamepad_Stick_Left(int nPlayer);
D3DXVECTOR3 GetGamepad_Stick_Right(int nPlayer);
float GetGamepad_Trigger_Left(int nPlayer);
float GetGamepad_Trigger_Right(int nPlayer);

//マウス用
HRESULT InitMouse(HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool GetMousePress(MOUSE_PUSH nKey);
D3DXVECTOR3 GetMouseMove(void);

int GetMouseDebug0(void);
int GetMouseDebug1(void);
int GetMouseDebug2(void);
int GetMouseDebug3(void);
int GetMouseDebug4(void);
int GetMouseDebug5(void);
int GetMouseDebug6(void);
int GetMouseDebug7(void);
#endif
