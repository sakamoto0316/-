#include "main.h"
#include "debugproc.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#define MODEL_SPEED (2.5f)	//デバッグのスピード

//グローバル変数
LPD3DXFONT g_pFont = NULL;					//フォントへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDebugDevice = NULL;		//Direct3Dデバイスへのポインタ
char g_aStrDebug[2048];						//文字列(デバッグ情報)を格納するバッファ

//====================================================================
//デバッグの初期化処理
//====================================================================
void InitDebugProc(void)
{
	g_pD3DDebugDevice = GetDevice();

	//デバック表示用フォントの生成
	D3DXCreateFont(g_pD3DDebugDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "terminal", &g_pFont);

	//デバッグ情報バッファのクリア
	memset(&g_aStrDebug[0], 0, sizeof g_aStrDebug);
}

//====================================================================
//デバッグの終了処理
//====================================================================
void UninitDebugProc(void)
{
	//デバック表示用フォントの破棄
	if (g_pD3DDebugDevice != NULL)
	{
		g_pD3DDebugDevice->Release();
		g_pD3DDebugDevice = NULL;
	}
}

//====================================================================
//デバッグの更新処理
//====================================================================
void UpdateDebugProc(void)
{

}

//====================================================================
//デバッグの描画処理
//====================================================================
void DrawDebugProc(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//テキストの描画
	g_pFont->DrawText(NULL, &g_aStrDebug[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	//デバッグ情報バッファのクリア
	memset(&g_aStrDebug[0], 0, sizeof g_aStrDebug);
}
//====================================================================
//デバッグ情報設定
//====================================================================
void PrintDebugProc(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	for (const char* p = fmt; *p != '\0'; ++p) {
		switch (*p) {
		case 'd':
			printf("%d ", va_arg(args, int));
			break;
		case 'f':
			printf("%f ", va_arg(args, double));
			break;
		case 'c':
			printf("%c ", va_arg(args, char));
			break;
		case 's':
			printf("%s ", va_arg(args, const char*));
			break;
		default:
			assert(!"不正な変換指定");
			break;
		}
	}
	printf("\n");

	va_end(args);
}