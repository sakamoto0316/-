#include "main.h"
#include "debugproc.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#define MODEL_SPEED (2.5f)	//�f�o�b�O�̃X�s�[�h

//�O���[�o���ϐ�
LPD3DXFONT g_pFont = NULL;					//�t�H���g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDebugDevice = NULL;		//Direct3D�f�o�C�X�ւ̃|�C���^
char g_aStrDebug[2048];						//������(�f�o�b�O���)���i�[����o�b�t�@

//====================================================================
//�f�o�b�O�̏���������
//====================================================================
void InitDebugProc(void)
{
	g_pD3DDebugDevice = GetDevice();

	//�f�o�b�N�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDebugDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "terminal", &g_pFont);

	//�f�o�b�O���o�b�t�@�̃N���A
	memset(&g_aStrDebug[0], 0, sizeof g_aStrDebug);
}

//====================================================================
//�f�o�b�O�̏I������
//====================================================================
void UninitDebugProc(void)
{
	//�f�o�b�N�\���p�t�H���g�̔j��
	if (g_pD3DDebugDevice != NULL)
	{
		g_pD3DDebugDevice->Release();
		g_pD3DDebugDevice = NULL;
	}
}

//====================================================================
//�f�o�b�O�̍X�V����
//====================================================================
void UpdateDebugProc(void)
{

}

//====================================================================
//�f�o�b�O�̕`�揈��
//====================================================================
void DrawDebugProc(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &g_aStrDebug[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	//�f�o�b�O���o�b�t�@�̃N���A
	memset(&g_aStrDebug[0], 0, sizeof g_aStrDebug);
}
//====================================================================
//�f�o�b�O���ݒ�
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
			assert(!"�s���ȕϊ��w��");
			break;
		}
	}
	printf("\n");

	va_end(args);
}