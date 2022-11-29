#include "input.h"
#include <stdio.h>

//マクロ定義
#define GAMEPAD_TRIGGER_DEADZONE (100)	//トリガーのデッドゾーン
#define TRIGGER_MAX (255)	//トリガーのマックス
#define NUM_KEY_MAX (256)	//キーの最大数
#define MAX_PLAYER (4)	//プレイヤーの人数

#define MOUSE_SENS (0.1f) //マウス感度の補正

//キーボード
typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevKeyboard = NULL; //入力デバイスへのポインタ
	BYTE aKeyState[NUM_KEY_MAX]; //全入力情報の保管
	BYTE aKeyStateTrigger[NUM_KEY_MAX]; //トリガー情報
	BYTE aKeyStateRerease[NUM_KEY_MAX]; //リリース情報
	BYTE aKeyStateRepeat[NUM_KEY_MAX]; //リピート情報
}KeyBoard;

//マウス
typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevMouse = NULL; //入力デバイスへのポインタ
	DIMOUSESTATE2 MouseState; //全入力情報の保管
	DIMOUSESTATE2 MouseStateTrigger; //トリガー情報
	DIMOUSESTATE2 MouseStateRerease; //リリース情報
	DIMOUSESTATE2 MouseStateRepeat; //リピート情報
	BYTE aOldState[8]; //前回の入力情報を保存
}Mouse;

//ジョイパッド
typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevJoypad = NULL; //入力デバイスへのポインタ
	DIJOYSTATE aKeyState; //全入力情報の保管
	DIJOYSTATE aKeyStateTrigger; //トリガー情報
	DIJOYSTATE aKeyStateRerease; //リリース情報
	DIJOYSTATE aKeyStateRepeat; //リピート情報
	BYTE aOldState[32]; //前回の入力情報を保存
}JoyPad;

//グローバル変数
LPDIRECTINPUT8 g_pInput = NULL;				//DirectInputオブジェクトへのポインタ
KeyBoard g_KeyBoard; //キーボードの情報
JoyPad g_aJoyPad[MAX_PLAYER]; //ジョイパッドの情報
Mouse g_Mouse; //マウスの情報
int g_nJoyNumCnt; //使用するジョイパッドのカウント

XINPUT_STATE g_JoyKeyState[MAX_PLAYER];
XINPUT_STATE g_JoyKeyStateTrigger[MAX_PLAYER];
XINPUT_STATE g_JoyKeyStateRelease[MAX_PLAYER];

//==========================================
//  デバイスの初期化
//==========================================
HRESULT InitDevice(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (g_pInput == nullptr)
	{
		if (FAILED(DirectInput8Create
		(
			hInstance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&g_pInput,
			NULL
		)))
		{
			return E_FAIL;
		}
	}

	//キーボード
	if (FAILED(InitKeyboard(hWnd)))
	{
		return E_FAIL;
	}

	//ジョイパッド
	InitGamepad(hWnd);

	//マウス
	InitMouse(hWnd);

	return S_OK;
}

//==========================================
//  デバイスの終了処理
//==========================================
void UninitDevice(void)
{
	//キーボード
	UninitKeyboard();

	//ジョイパッド
	UninitGamepad();

	//マウス
	UninitMouse();

	//DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//==========================================
//  デバイスの更新処理
//==========================================
void UpdateDevice(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//ジョイパッドの更新処理
	UpdateGamepad();

	//マウスの更新処理
	UpdateMouse();
}

//====================================================================
//キーボードの初期化処理
//====================================================================
HRESULT InitKeyboard(HWND hWnd)
{
	//入力デバイスの設定
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_KeyBoard.pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_KeyBoard.pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_KeyBoard.pDevKeyboard->SetCooperativeLevel
	(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	)))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_KeyBoard.pDevKeyboard->Acquire();

	return S_OK;
}

//====================================================================
//キーボードの終了処理
//====================================================================
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (g_KeyBoard.pDevKeyboard != NULL)
	{
		g_KeyBoard.pDevKeyboard->Unacquire();
		g_KeyBoard.pDevKeyboard->Release();
		g_KeyBoard.pDevKeyboard = NULL;
	}
}

//====================================================================
//キーボードの更新処理
//====================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX]; //キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_KeyBoard.pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_KeyBoard.aKeyStateRerease[nCntKey] = ~aKeyState[nCntKey] & ((g_KeyBoard.aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_KeyBoard.aKeyState[nCntKey]); //キーボードのリリース情報を保存
			g_KeyBoard.aKeyStateTrigger[nCntKey] = (g_KeyBoard.aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey]; //キーボードのトリガー情報の保存
			g_KeyBoard.aKeyState[nCntKey] = aKeyState[nCntKey]; //キーボードのプレス情報の保存
		}
	}
	else
	{
		g_KeyBoard.pDevKeyboard->Acquire(); //キーボードのアクセス権
	}
}

//====================================================================
//キーボードのプレス情報を所得
//====================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_KeyBoard.aKeyState[nKey] & 0x80) ? true : false;
}

//====================================================================
//キーボードのトリガー情報を所得
//====================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_KeyBoard.aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//====================================================================
//キーボードのリリース情報を所得
//====================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_KeyBoard.aKeyStateRerease[nKey] & 0x80) ? true : false;
}

//====================================================================
//ゲームパッドの初期化処理
//====================================================================
HRESULT InitGamepad(HWND hWnd)
{
	//Xinputのステートの設定
	XInputEnable(true);
	int nCount;

	for (nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		//メモリーをクリアする
		memset(&g_JoyKeyState[nCount], 0, sizeof(XINPUT_STATE));
		memset(&g_JoyKeyStateTrigger[nCount], 0, sizeof(XINPUT_STATE));
		memset(&g_JoyKeyStateRelease[nCount], 0, sizeof(XINPUT_STATE));
	}
	return S_OK;
}

//====================================================================
//ゲームパッドの終了処理
//====================================================================
void UninitGamepad(void)
{
	//Xinputのステートの設定
	XInputEnable(false);
}

//====================================================================
//ゲームパッドの更新処理
//====================================================================
void UpdateGamepad(void)
{
	XINPUT_STATE aGamepadState[MAX_PLAYER];	//ゲームパッドの入力処理
	static int nCountSpece = 0;
	int nCntPad = 0;

	for (nCntPad = 0; nCntPad < MAX_PLAYER; nCntPad++)
	{
		//入力デバイスからデータを所得
		if (XInputGetState(nCntPad, &aGamepadState[nCntPad]) == ERROR_SUCCESS)
		{
			g_JoyKeyStateTrigger[nCntPad].Gamepad.wButtons =
				~g_JoyKeyState[nCntPad].Gamepad.wButtons
				& aGamepadState[nCntPad].Gamepad.wButtons;	//トリガー

			g_JoyKeyStateRelease[nCntPad].Gamepad.wButtons =
				((g_JoyKeyState[nCntPad].Gamepad.wButtons
					^ aGamepadState[nCntPad].Gamepad.wButtons)
					& g_JoyKeyState[nCntPad].Gamepad.wButtons);	//リリース

			g_JoyKeyStateTrigger[nCntPad].Gamepad.bLeftTrigger =
				~g_JoyKeyState[nCntPad].Gamepad.bLeftTrigger
				& aGamepadState[nCntPad].Gamepad.bLeftTrigger;	//トリガー

			g_JoyKeyStateTrigger[nCntPad].Gamepad.bRightTrigger =
				~g_JoyKeyState[nCntPad].Gamepad.bRightTrigger
				& aGamepadState[nCntPad].Gamepad.bRightTrigger;	//トリガー

																//g_JoyKeyStateTrigger[nCntPad].Gamepad.bRightTrigger = aGamepadState[nCntPad].Gamepad.bRightTrigger;	//プレス

			g_JoyKeyStateTrigger[nCntPad].Gamepad.sThumbLY =
				~g_JoyKeyState[nCntPad].Gamepad.sThumbLY
				& aGamepadState[nCntPad].Gamepad.sThumbLY;	//トリガー

			g_JoyKeyStateTrigger[nCntPad].Gamepad.sThumbLX =
				~g_JoyKeyState[nCntPad].Gamepad.sThumbLX
				& aGamepadState[nCntPad].Gamepad.sThumbLX;	//トリガー

			g_JoyKeyStateRelease[nCntPad].Gamepad.sThumbLY =
				((g_JoyKeyState[nCntPad].Gamepad.sThumbLY
					^ aGamepadState[nCntPad].Gamepad.sThumbLY)
					& g_JoyKeyState[nCntPad].Gamepad.sThumbLY);	//リリース

			g_JoyKeyState[nCntPad] = aGamepadState[nCntPad];	//プレス
		}
	}
}

//====================================================================
//ゲームパッドのプレス情報を所得
//====================================================================
bool GetGamepadPress(JOYKEY key, int nPlayer)
{
	return (g_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//====================================================================
//ゲームパッドのトリガー情報を所得
//====================================================================
bool GetGamepadTrigger(JOYKEY key, int nPlayer)
{
	return (g_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//====================================================================
//ゲームパッドのリリース情報を所得
//====================================================================
bool GetGamepadRelease(JOYKEY key, int nPlayer)
{
	return (g_JoyKeyStateRelease[nPlayer].Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//====================================================================
//ゲームパッドのLスティックの情報を所得
//====================================================================
D3DXVECTOR3 GetGamepad_Stick_Left(int nPlayer)
{
	//ローカル変数宣言
	D3DXVECTOR3 Stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Stick.x = (float)g_JoyKeyState[nPlayer].Gamepad.sThumbLX / (float)SHRT_MAX;
	if (fabsf(Stick.x) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / (float)SHRT_MAX)
	{
		Stick.x = 0.0f;
	}

	Stick.y = (float)g_JoyKeyState[nPlayer].Gamepad.sThumbLY / (float)SHRT_MAX;
	if (fabsf(Stick.y) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / (float)SHRT_MAX)
	{
		Stick.y = 0.0f;
	}

	return Stick;
}

//====================================================================
//ゲームパッドのRスティックの情報を所得
//====================================================================
D3DXVECTOR3 GetGamepad_Stick_Right(int nPlayer)
{
	//ローカル変数宣言
	D3DXVECTOR3 Stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Stick.x = (float)g_JoyKeyState[nPlayer].Gamepad.sThumbRX / (float)SHRT_MAX;
	if (fabsf(Stick.x) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / (float)SHRT_MAX)
	{
		Stick.x = 0.0f;
	}

	Stick.y = (float)g_JoyKeyState[nPlayer].Gamepad.sThumbRY / (float)SHRT_MAX;
	if (fabsf(Stick.y) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / (float)SHRT_MAX)
	{
		Stick.y = 0.0f;
	}

	return Stick;
}

//====================================================================
//ゲームパッドのLトリガーの情報を所得
//====================================================================
float GetGamepad_Trigger_Left(int nPlayer)
{
	//ローカル変数宣言
	float Trigger = 0.0f;

	Trigger = (float)g_JoyKeyState[nPlayer].Gamepad.bLeftTrigger / (float)TRIGGER_MAX;
	if (fabsf(Trigger) < (float)GAMEPAD_TRIGGER_DEADZONE / (float)TRIGGER_MAX)
	{
		Trigger = 0.0f;
	}

	return Trigger;
}

//====================================================================
//ゲームパッドのRトリガーの情報を所得
//====================================================================
float GetGamepad_Trigger_Right(int nPlayer)
{
	//ローカル変数宣言
	float Trigger = 0.0f;

	Trigger = (float)g_JoyKeyState[nPlayer].Gamepad.bRightTrigger / (float)TRIGGER_MAX;
	if (fabsf(Trigger) < (float)GAMEPAD_TRIGGER_DEADZONE / (float)TRIGGER_MAX)
	{
		Trigger = 0.0f;
	}

	return Trigger;
}

//==========================================
//  マウスの初期化
//==========================================
HRESULT InitMouse(HWND hWnd)
{
	//入力デバイスの設定
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_Mouse.pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_Mouse.pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_Mouse.pDevMouse->SetCooperativeLevel
	(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	)))
	{
		return E_FAIL;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; //相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (FAILED(g_Mouse.pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		// デバイスの設定に失敗
		return E_FAIL;
	}

	//マウスへのアクセス権を取得
	g_Mouse.pDevMouse->Acquire();

	//マウスカーソルの消去
	ShowCursor(false);

	return S_OK;
}

//==========================================
//  マウスの終了処理
//==========================================
void UninitMouse(void)
{
	//入力デバイス(マウス)の破棄
	if (g_Mouse.pDevMouse != NULL)
	{
		g_Mouse.pDevMouse->Unacquire();
		g_Mouse.pDevMouse->Release();
		g_Mouse.pDevMouse = NULL;
	}
}

//==========================================
//  マウスの更新処理
//==========================================
void UpdateMouse(void)
{
	DIMOUSESTATE2 mouse; //マウスの入力情報

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_Mouse.pDevMouse->GetDeviceState(sizeof(mouse), &mouse)))
	{
		//入力情報の保存
		g_Mouse.MouseState = mouse;
	}
	else
	{
		g_Mouse.pDevMouse->Acquire(); //マウスへのアクセス権
	}
}

//==========================================
//  マウスのプレス情報
//==========================================
bool GetMousePress(MOUSE_PUSH nKey)
{
	return (g_Mouse.MouseState.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================
//  マウスの移動量
//==========================================
D3DXVECTOR3 GetMouseMove(void)
{
	return D3DXVECTOR3(((float)g_Mouse.MouseState.lX) * MOUSE_SENS, ((float)g_Mouse.MouseState.lY) * MOUSE_SENS, 0.0f);
}

int GetMouseDebug0(void)
{
	return g_Mouse.MouseState.rgbButtons[0];
}
int GetMouseDebug1(void)
{
	return g_Mouse.MouseState.rgbButtons[1];
}
int GetMouseDebug2(void)
{
	return g_Mouse.MouseState.rgbButtons[2];
}
int GetMouseDebug3(void)
{
	return g_Mouse.MouseState.rgbButtons[3];
}
int GetMouseDebug4(void)
{
	return g_Mouse.MouseState.rgbButtons[4];
}
int GetMouseDebug5(void)
{
	return g_Mouse.MouseState.rgbButtons[5];
}
int GetMouseDebug6(void)
{
	return g_Mouse.MouseState.rgbButtons[6];
}
int GetMouseDebug7(void)
{
	return g_Mouse.MouseState.rgbButtons[7];
}