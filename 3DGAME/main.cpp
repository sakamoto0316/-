//============================================
//
//アクションゲーム[main.cpp]
//Author:sakamoto kai
//
//============================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "shadow.h"
#include "input.h"
#include "billboard.h"
#include "effect.h"
#include "explosion.h"
#include "debugproc.h"
#include "particle.h"
#include "polygon.h"
#include "meshfieldidx.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "wall.h"
#include "model.h"
#include "object00.h"
#include "sound.h"
#include <stdio.h>

//マクロ定義
#define CLASS_NAME "ACTION"			//ウインドウクラスの名前
#define WINDOW_NAME "アクションゲーム"	//ウインドウの名前(キャプションに表示)

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//グローバル変数
LPDIRECT3D9 g_pD3D = NULL;					//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		//Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFontM = NULL;					//フォントへのポインタ
int g_nCountFPS = 0;						//FPSカウンタ
bool g_bDispDebug = false;					//デバッグ表示のON/OFF
MODE g_mode = MODE_TITLE;					//現在のモード
bool g_WireFrame = false;					//ワイヤーフレームのオンオフ

//============================================
//メイン関数
//============================================
int	WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hinstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時刻
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwFPSLastTime;	//最後にFPSを計測した時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							//ウインドウのスタイル
		WindowProc,							//ウインドウプロシージャ
		0,									//0にする(通常は使用しない)
		0,									//0にする(通常は使用しない)
		hInstance,							//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),		//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),			//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			//クライアント領域の背景色
		NULL,								//メニューバー
		CLASS_NAME,							//ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION),		//ファイルのアイコン
	};
	HWND hWnd;
	MSG msg;
	RECT rect = { 0,0,SCREEN_WIDTH ,SCREEN_HEIGHT };

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(0,	//拡張ウインドウスタイル
		CLASS_NAME,				//ウインドウクラスの名前
		WINDOW_NAME,			//ウインドウの名前
		WS_OVERLAPPEDWINDOW,	//ウインドウスタイル
		CW_USEDEFAULT,			//ウインドウの左上X座標
		CW_USEDEFAULT,			//ウインドウの左上Y座標
		SCREEN_WIDTH,			//ウインドウの幅
		SCREEN_HEIGHT,			//ウインドウの高さ
		NULL,					//親ウインドウのハンドル
		NULL,					//メニューハンドルまたは子ウインドウID
		hInstance,				//インスタンスハンドル
		NULL);					//ウインドウ作成データ
								//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return -1;
	}

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;				//初期化する
	dwExecLastTime = timeGetTime();	//現在時刻を所得(保存)

	srand(dwExecLastTime);

	//メッセージループ
	while (1)
	{//Windowsの処理
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);	//ウインドウの表示状態を設定
				DispatchMessage(&msg);	//クライアント領域を更新
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();		//現在時刻を所得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
			 //FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	//FPSを計測した時刻を保存
				dwFrameCount = 0;				//フレームカウントをクリア
			}

			if (dwCurrentTime - dwExecLastTime >= (1000 / 60))
			{//６０分の１秒経過
				dwExecLastTime = dwCurrentTime;	//処理開始の時刻[現在時刻]を保存

												//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;					//フレームカウントを加算
			}
		}
	}

	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//====================================================================
//ウインドウプロシージャ
//====================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char aString[256] = "test";		//描画するテキストを格納
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//ウインドウの領域（矩形）

	int nID;						//返り値を格納
	HWND hWndButtonFinish = 0;			//終了ボタンのウインドウハンドル(識別子)

	switch (uMsg)
	{
	case WM_DESTROY:	//ウインドウ破棄のメッセージ
						//WM_QUITメッセージを送る。
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:	//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE: //[ESC]キーが押された
			nID = MessageBox(NULL, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				//ウインドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
				break;
			}
			break;
		}

		//終了ボタンに表示する文字列を設定
		SetWindowText(hWndButtonFinish, "終了");

		break;

	case WM_CLOSE:	//閉じる押下のメッセージ
		nID = MessageBox(NULL, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{
			//ウインドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;	//０を返さないと終了してしまう
		}

		break;

	case WM_LBUTTONDOWN: //マウス左クリックのメッセージ
						 //対象ウインドウにフォーカスを合わせる
		SetFocus(hWnd);
		break;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	//既定の処理を返す
}

//====================================================================
//初期化処理
//====================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{

	D3DDISPLAYMODE d3ddm;	//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ

									//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを所得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));	//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//ゲーム画面のサイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//ゲーム画面のサイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							//バックバッファの形式
	d3dpp.BackBufferCount = 1;										//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;							//デプスバッファとシテンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//デプスバッファを16bitとして扱う
	d3dpp.Windowed = bWindow;										//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル

																	//Direct3dデバイスの作成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステート(テクスチャの拡縮補完)の設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステート(テクスチャのアルファブレンド)の設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//デバック表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "terminal", &g_pFontM);

	//デバイスの初期化処理
	if (FAILED(InitDevice(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	////デバッグ情報の初期化処理
	//InitDebugProc();

	//カメラの初期化処理
	InitCamera();

	//ライトの初期化処理
	InitLight();

	//影の初期化処理
	InitShadow();

	//ビルボードの初期化処理
	InitBillboard();

	//爆発の初期化処理
	InitExplosion();

	//エフェクトの初期化処理
	InitEffect();

	//パーティクルの初期化処理
	InitParticle();

	//メッシュフィールドの初期化処理
	InitMeshFieldIdx();

	//メッシュウォールの初期化処理
	InitMeshWall();

	//メッシュシリンダーの初期化処理
	InitMeshCylinder();

	//メッシュドームの初期化処理
	InitMeshDome();

	//ポリゴンの初期化処理
	InitPolygon();

	//壁の初期化処理
	InitWall();

	//モデルの初期化処理
	InitModel();

	//オブジェクトの初期化処理
	InitObject00();

	////サウンドの初期化処理
	//InitSound(hWnd);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void Uninit(void)
{
	//メッシュフィールドの終了処理
	UninitMeshFieldIdx();

	//メッシュウォールの終了処理
	UninitMeshWall();

	//メッシュシリンダーの終了処理
	UninitMeshCylinder();

	//メッシュドームの終了処理
	UninitMeshDome();

	//ポリゴンの終了処理
	UninitPolygon();

	//壁の終了処理
	UninitWall();

	//オブジェクトの終了処理
	UninitObject00();

	//モデルの終了処理
	UninitModel();

	//ビルボードの終了処理
	UninitBillboard();

	//エフェクトの終了処理
	UninitEffect();

	//パーティクルの終了処理
	UninitParticle();

	//爆発の終了処理
	UninitExplosion();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	//影の終了処理
	UninitShadow();

	////サウンドの終了処理
	//UninitSound();

	//デバイスの終了処理
	UninitDevice();

	////デバッグ情報の終了処理
	//UninitDebugProc();

	//デバック表示用フォントの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dデバイスの破棄
	if (g_pFontM != NULL)
	{
		g_pFontM->Release();
		g_pFontM = NULL;
	}
	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//====================================================================
//更新処理
//====================================================================
void Update(void)
{
	////デバッグ情報の更新処理
	//UpdateDebugProc();

	//デバイスの更新
	UpdateDevice();

	//カメラの更新処理
	UpdateCamera();

	//ライトの更新処理
	UpdateLight();

	//影の更新処理
	UpdateShadow();

	//モデルの更新処理
	UpdateModel();

	//オブジェクトの更新処理
	UpdateObject00();

	//ビルボードの更新処理
	UpdateBillboard();

	//エフェクトの更新処理
	UpdateEffect();

	//パーティクルの更新処理
	UpdateParticle();

	//爆発の更新処理
	UpdateExplosion();

	//メッシュフィールドの更新処理
	UpdateMeshFieldIdx();

	//メッシュウォールの更新処理
	UpdateMeshWall();

	//メッシュシリンダーの更新処理
	UpdateMeshCylinder();

	//メッシュドームの更新処理
	UpdateMeshDome();

	//ポリゴンの更新処理
	UpdatePolygon();

	//壁の更新処理
	UpdateWall();

	if (g_WireFrame == true)
	{
		//レンダーステートの設定
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{
		//レンダーステートの設定
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, 0);
	}

	if (GetKeyboardTrigger(DIK_0) == true)
	{
		g_WireFrame = (g_WireFrame == true) ? false : true;
	}

	switch (g_mode)
	{
	case MODE_TITLE:
		break;
	case MODE_TUTORIAL:
		break;
	case MODE_GAME:
		break;
	case MODE_RESULT:
		break;
	}
}

//====================================================================
//描画処理
//====================================================================
void Draw(void)
{
	//画面クリア(バックバッファとZバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画処理が成功した場合

		switch (g_mode)
		{
		case MODE_TITLE:
			break;
		case MODE_TUTORIAL:
			break;
		case MODE_GAME:
			break;
		case MODE_RESULT:
			break;
		}

		//カメラの設定
		SetCamera();

		//ポリゴンの描画処理
		DrawPolygon();

		//メッシュフィールドの描画処理
		DrawMeshFieldIdx();

		//メッシュウォールの描画処理
		DrawMeshWall();

		//メッシュシリンダーの描画処理
		DrawMeshCylinder();

		//メッシュドームの描画処理
		DrawMeshDome();

		//壁の描画処理
		DrawWall();

		//モデルの描画処理
		DrawModel();

		//オブジェクトの描画処理
		DrawObject00();

		//影の描画処理
		DrawShadow();

		//パーティクルの描画処理
		DrawParticle();

		//エフェクトの描画処理
		DrawEffect();

		//ビルボードの描画処理
		DrawBillboard();

		//爆発の描画処理
		DrawExplosion();

		////デバッグ情報の描画処理
		//DrawDebugProc();

		if (g_bDispDebug == false)
		{
			//FPSの表示
			DrawFPS();
		}

		if (GetKeyboardTrigger(DIK_F1) == true)
		{//F1キーが押された
			g_bDispDebug = (g_bDispDebug == true) ? false : true;
		}

		//描画終了
		g_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//====================================================================
//デバイスの所得
//====================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//====================================================================
//FPSの表示
//====================================================================
void DrawFPS(void)
{
	Model * pModel = GetModel();

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];
	char aStick[256];
	char aposV[256];
	char aposR[256];
	char avecU[256];
	char arot[256];
	char aCameraMode[256];
	char sousa[256];
	char modelrot[256];
	char modelpos[256];
	char aaa0[256];
	char aaa1[256];
	char aaa2[256];
	char aaa3[256];
	char aaa4[256];
	char aaa5[256];
	char aaa6[256];
	char aaa7[256];
	char effect[256];

	//文字列に代入
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);
	sprintf(&aStick[0], "左スティック ( %.3f : %.3f )", GetGamepad_Stick_Left(0).x, GetGamepad_Stick_Left(0).y);
	sprintf(&aposV[0], "視点 ( %.3f : %.3f : %.3f )", GetposV().x, GetposV().y, GetposV().z);
	sprintf(&aposR[0], "注視点 ( %.3f : %.3f : %.3f )", GetposR().x, GetposR().y, GetposR().z);
	sprintf(&avecU[0], "上方向ベクトル ( %.3f : %.3f : %.3f )", GetvecU().x, GetvecU().y, GetvecU().z);
	sprintf(&arot[0], "カメラの向き ( %.3f : %.3f : %.3f )", Getrot().x, Getrot().y, Getrot().z);
	sprintf(&modelrot[0], "モデルの向き ( %.3f : %.3f : %.3f )", pModel->rot.x, pModel->rot.y, pModel->rot.z);
	sprintf(&modelpos[0], "モデルの位置 ( %.3f : %.3f : %.3f )", pModel->pos.x, pModel->pos.y, pModel->pos.z);
	sprintf(&sousa[0], "左スティックでモデル移動\n右スティックで視点操作\n右トリガーで視点上昇\n左トリガーで視点下降\n\n[WASD]でモデル移動\n[EQ]でモデル旋回\n[IJKL]で視点移動\n左の[SHIFT]と[CTRL]でカメラの上昇下降\n右の[SHIFT]と[CTRL]でモデルの上昇下降\n[SPACE]で弾発射");
	sprintf(&aaa0[0], "マウスの情報:%d", GetMouseDebug0());
	sprintf(&aaa1[0], "マウスの情報:%d", GetMouseDebug1());
	sprintf(&aaa2[0], "マウスの情報:%d", GetMouseDebug2());
	sprintf(&aaa3[0], "マウスの情報:%d", GetMouseDebug3());
	sprintf(&aaa4[0], "マウスの情報:%d", GetMouseDebug4());
	sprintf(&aaa5[0], "マウスの情報:%d", GetMouseDebug5());
	sprintf(&aaa6[0], "マウスの情報:%d", GetMouseDebug6());
	sprintf(&aaa7[0], "マウスの情報:%d", GetMouseDebug7());
	sprintf(&effect[0], "エフェクトの数( %d )", GetEffectNum());

		switch (GetCameraMode())
		{
		case CAMERA_MODE_1:
			sprintf(&aCameraMode[0], "Rスティック押し込み or [F3] ( カメラモード : 一人称視点 )");
			break;
		case CAMERA_MODE_3:
			sprintf(&aCameraMode[0], "Rスティック押し込み or [F3] ( カメラモード : 三人称視点 )");
			break;
		}

	//テキストの描画
	g_pFontM->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &aStick[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &aposV[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &aposR[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &avecU[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &arot[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &modelrot[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &modelpos[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &aCameraMode[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &effect[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	rect = { 950,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	g_pFontM->DrawText(NULL, &sousa[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 300;
	g_pFontM->DrawText(NULL, &aaa0[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &aaa1[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &aaa2[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &aaa3[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &aaa4[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &aaa5[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &aaa6[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 17;
	g_pFontM->DrawText(NULL, &aaa7[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//====================================================================
//モードの設定
//====================================================================
void SetMode(MODE mode)
{
	//現在の画面(モード)の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:
		break;
	case MODE_TUTORIAL:
		break;
	case MODE_GAME:
		break;
	case MODE_RESULT:
		break;
	}

	g_mode = mode;	//モードを設定する

					//新しい画面(モード)の初期化処理
	switch (mode)
	{
	case MODE_TITLE:
		break;
	case MODE_TUTORIAL:
		break;
	case MODE_GAME:
		break;
	case MODE_RESULT:
		break;
	}
}

//====================================================================
//モードを所得
//====================================================================
MODE GetMode(void)
{
	return g_mode;
}