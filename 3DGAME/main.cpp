//============================================
//
//�A�N�V�����Q�[��[main.cpp]
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

//�}�N����`
#define CLASS_NAME "ACTION"			//�E�C���h�E�N���X�̖��O
#define WINDOW_NAME "�A�N�V�����Q�[��"	//�E�C���h�E�̖��O(�L���v�V�����ɕ\��)

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//�O���[�o���ϐ�
LPDIRECT3D9 g_pD3D = NULL;					//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		//Direct3D�f�o�C�X�ւ̃|�C���^
LPD3DXFONT g_pFontM = NULL;					//�t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;						//FPS�J�E���^
bool g_bDispDebug = false;					//�f�o�b�O�\����ON/OFF
MODE g_mode = MODE_TITLE;					//���݂̃��[�h
bool g_WireFrame = false;					//���C���[�t���[���̃I���I�t

//============================================
//���C���֐�
//============================================
int	WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hinstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;	//���ݎ���
	DWORD dwExecLastTime;	//�Ō�ɏ�����������
	DWORD dwFrameCount;		//�t���[���J�E���g
	DWORD dwFPSLastTime;	//�Ō��FPS���v����������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,							//�E�C���h�E�̃X�^�C��
		WindowProc,							//�E�C���h�E�v���V�[�W��
		0,									//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,									//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance,							//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),		//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),			//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),			//�N���C�A���g�̈�̔w�i�F
		NULL,								//���j���[�o�[
		CLASS_NAME,							//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION),		//�t�@�C���̃A�C�R��
	};
	HWND hWnd;
	MSG msg;
	RECT rect = { 0,0,SCREEN_WIDTH ,SCREEN_HEIGHT };

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,	//�g���E�C���h�E�X�^�C��
		CLASS_NAME,				//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,			//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,	//�E�C���h�E�X�^�C��
		CW_USEDEFAULT,			//�E�C���h�E�̍���X���W
		CW_USEDEFAULT,			//�E�C���h�E�̍���Y���W
		SCREEN_WIDTH,			//�E�C���h�E�̕�
		SCREEN_HEIGHT,			//�E�C���h�E�̍���
		NULL,					//�e�E�C���h�E�̃n���h��
		NULL,					//���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,				//�C���X�^���X�n���h��
		NULL);					//�E�C���h�E�쐬�f�[�^
								//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������������s�����ꍇ
		return -1;
	}

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;				//����������
	dwExecLastTime = timeGetTime();	//���ݎ���������(�ۑ�)

	srand(dwExecLastTime);

	//���b�Z�[�W���[�v
	while (1)
	{//Windows�̏���
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烋�[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);	//�E�C���h�E�̕\����Ԃ�ݒ�
				DispatchMessage(&msg);	//�N���C�A���g�̈���X�V
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();		//���ݎ���������

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
			 //FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	//FPS���v������������ۑ�
				dwFrameCount = 0;				//�t���[���J�E���g���N���A
			}

			if (dwCurrentTime - dwExecLastTime >= (1000 / 60))
			{//�U�O���̂P�b�o��
				dwExecLastTime = dwCurrentTime;	//�����J�n�̎���[���ݎ���]��ۑ�

												//�X�V����
				Update();

				//�`�揈��
				Draw();

				dwFrameCount++;					//�t���[���J�E���g�����Z
			}
		}
	}

	//�I������
	Uninit();

	//����\��߂�
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//====================================================================
//�E�C���h�E�v���V�[�W��
//====================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char aString[256] = "test";		//�`�悷��e�L�X�g���i�[
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//�E�C���h�E�̗̈�i��`�j

	int nID;						//�Ԃ�l���i�[
	HWND hWndButtonFinish = 0;			//�I���{�^���̃E�C���h�E�n���h��(���ʎq)

	switch (uMsg)
	{
	case WM_DESTROY:	//�E�C���h�E�j���̃��b�Z�[�W
						//WM_QUIT���b�Z�[�W�𑗂�B
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:	//�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE: //[ESC]�L�[�������ꂽ
			nID = MessageBox(NULL, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{
				//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
				break;
			}
			break;
		}

		//�I���{�^���ɕ\�����镶�����ݒ�
		SetWindowText(hWndButtonFinish, "�I��");

		break;

	case WM_CLOSE:	//���鉟���̃��b�Z�[�W
		nID = MessageBox(NULL, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

		if (nID == IDYES)
		{
			//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;	//�O��Ԃ��Ȃ��ƏI�����Ă��܂�
		}

		break;

	case WM_LBUTTONDOWN: //�}�E�X���N���b�N�̃��b�Z�[�W
						 //�ΏۃE�C���h�E�Ƀt�H�[�J�X�����킹��
		SetFocus(hWnd);
		break;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	//����̏�����Ԃ�
}

//====================================================================
//����������
//====================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{

	D3DDISPLAYMODE d3ddm;	//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^

									//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//���݂̃f�B�X�v���C���[�h������
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));	//�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//�Q�[����ʂ̃T�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//�Q�[����ʂ̃T�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;							//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;										//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;							//�f�v�X�o�b�t�@�ƃV�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//�f�v�X�o�b�t�@��16bit�Ƃ��Ĉ���
	d3dpp.Windowed = bWindow;										//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�C���^�[�o��

																	//Direct3d�f�o�C�X�̍쐬
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
	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g(�e�N�X�`���̊g�k�⊮)�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g(�e�N�X�`���̃A���t�@�u�����h)�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�f�o�b�N�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "terminal", &g_pFontM);

	//�f�o�C�X�̏���������
	if (FAILED(InitDevice(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	////�f�o�b�O���̏���������
	//InitDebugProc();

	//�J�����̏���������
	InitCamera();

	//���C�g�̏���������
	InitLight();

	//�e�̏���������
	InitShadow();

	//�r���{�[�h�̏���������
	InitBillboard();

	//�����̏���������
	InitExplosion();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�p�[�e�B�N���̏���������
	InitParticle();

	//���b�V���t�B�[���h�̏���������
	InitMeshFieldIdx();

	//���b�V���E�H�[���̏���������
	InitMeshWall();

	//���b�V���V�����_�[�̏���������
	InitMeshCylinder();

	//���b�V���h�[���̏���������
	InitMeshDome();

	//�|���S���̏���������
	InitPolygon();

	//�ǂ̏���������
	InitWall();

	//���f���̏���������
	InitModel();

	//�I�u�W�F�N�g�̏���������
	InitObject00();

	////�T�E���h�̏���������
	//InitSound(hWnd);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void Uninit(void)
{
	//���b�V���t�B�[���h�̏I������
	UninitMeshFieldIdx();

	//���b�V���E�H�[���̏I������
	UninitMeshWall();

	//���b�V���V�����_�[�̏I������
	UninitMeshCylinder();

	//���b�V���h�[���̏I������
	UninitMeshDome();

	//�|���S���̏I������
	UninitPolygon();

	//�ǂ̏I������
	UninitWall();

	//�I�u�W�F�N�g�̏I������
	UninitObject00();

	//���f���̏I������
	UninitModel();

	//�r���{�[�h�̏I������
	UninitBillboard();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�����̏I������
	UninitExplosion();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	//�e�̏I������
	UninitShadow();

	////�T�E���h�̏I������
	//UninitSound();

	//�f�o�C�X�̏I������
	UninitDevice();

	////�f�o�b�O���̏I������
	//UninitDebugProc();

	//�f�o�b�N�\���p�t�H���g�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�f�o�C�X�̔j��
	if (g_pFontM != NULL)
	{
		g_pFontM->Release();
		g_pFontM = NULL;
	}
	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//====================================================================
//�X�V����
//====================================================================
void Update(void)
{
	////�f�o�b�O���̍X�V����
	//UpdateDebugProc();

	//�f�o�C�X�̍X�V
	UpdateDevice();

	//�J�����̍X�V����
	UpdateCamera();

	//���C�g�̍X�V����
	UpdateLight();

	//�e�̍X�V����
	UpdateShadow();

	//���f���̍X�V����
	UpdateModel();

	//�I�u�W�F�N�g�̍X�V����
	UpdateObject00();

	//�r���{�[�h�̍X�V����
	UpdateBillboard();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//�p�[�e�B�N���̍X�V����
	UpdateParticle();

	//�����̍X�V����
	UpdateExplosion();

	//���b�V���t�B�[���h�̍X�V����
	UpdateMeshFieldIdx();

	//���b�V���E�H�[���̍X�V����
	UpdateMeshWall();

	//���b�V���V�����_�[�̍X�V����
	UpdateMeshCylinder();

	//���b�V���h�[���̍X�V����
	UpdateMeshDome();

	//�|���S���̍X�V����
	UpdatePolygon();

	//�ǂ̍X�V����
	UpdateWall();

	if (g_WireFrame == true)
	{
		//�����_�[�X�e�[�g�̐ݒ�
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{
		//�����_�[�X�e�[�g�̐ݒ�
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
//�`�揈��
//====================================================================
void Draw(void)
{
	//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`�揈�������������ꍇ

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

		//�J�����̐ݒ�
		SetCamera();

		//�|���S���̕`�揈��
		DrawPolygon();

		//���b�V���t�B�[���h�̕`�揈��
		DrawMeshFieldIdx();

		//���b�V���E�H�[���̕`�揈��
		DrawMeshWall();

		//���b�V���V�����_�[�̕`�揈��
		DrawMeshCylinder();

		//���b�V���h�[���̕`�揈��
		DrawMeshDome();

		//�ǂ̕`�揈��
		DrawWall();

		//���f���̕`�揈��
		DrawModel();

		//�I�u�W�F�N�g�̕`�揈��
		DrawObject00();

		//�e�̕`�揈��
		DrawShadow();

		//�p�[�e�B�N���̕`�揈��
		DrawParticle();

		//�G�t�F�N�g�̕`�揈��
		DrawEffect();

		//�r���{�[�h�̕`�揈��
		DrawBillboard();

		//�����̕`�揈��
		DrawExplosion();

		////�f�o�b�O���̕`�揈��
		//DrawDebugProc();

		if (g_bDispDebug == false)
		{
			//FPS�̕\��
			DrawFPS();
		}

		if (GetKeyboardTrigger(DIK_F1) == true)
		{//F1�L�[�������ꂽ
			g_bDispDebug = (g_bDispDebug == true) ? false : true;
		}

		//�`��I��
		g_pD3DDevice->EndScene();
	}
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//====================================================================
//�f�o�C�X�̏���
//====================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//====================================================================
//FPS�̕\��
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

	//������ɑ��
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);
	sprintf(&aStick[0], "���X�e�B�b�N ( %.3f : %.3f )", GetGamepad_Stick_Left(0).x, GetGamepad_Stick_Left(0).y);
	sprintf(&aposV[0], "���_ ( %.3f : %.3f : %.3f )", GetposV().x, GetposV().y, GetposV().z);
	sprintf(&aposR[0], "�����_ ( %.3f : %.3f : %.3f )", GetposR().x, GetposR().y, GetposR().z);
	sprintf(&avecU[0], "������x�N�g�� ( %.3f : %.3f : %.3f )", GetvecU().x, GetvecU().y, GetvecU().z);
	sprintf(&arot[0], "�J�����̌��� ( %.3f : %.3f : %.3f )", Getrot().x, Getrot().y, Getrot().z);
	sprintf(&modelrot[0], "���f���̌��� ( %.3f : %.3f : %.3f )", pModel->rot.x, pModel->rot.y, pModel->rot.z);
	sprintf(&modelpos[0], "���f���̈ʒu ( %.3f : %.3f : %.3f )", pModel->pos.x, pModel->pos.y, pModel->pos.z);
	sprintf(&sousa[0], "���X�e�B�b�N�Ń��f���ړ�\n�E�X�e�B�b�N�Ŏ��_����\n�E�g���K�[�Ŏ��_�㏸\n���g���K�[�Ŏ��_���~\n\n[WASD]�Ń��f���ړ�\n[EQ]�Ń��f������\n[IJKL]�Ŏ��_�ړ�\n����[SHIFT]��[CTRL]�ŃJ�����̏㏸���~\n�E��[SHIFT]��[CTRL]�Ń��f���̏㏸���~\n[SPACE]�Œe����");
	sprintf(&aaa0[0], "�}�E�X�̏��:%d", GetMouseDebug0());
	sprintf(&aaa1[0], "�}�E�X�̏��:%d", GetMouseDebug1());
	sprintf(&aaa2[0], "�}�E�X�̏��:%d", GetMouseDebug2());
	sprintf(&aaa3[0], "�}�E�X�̏��:%d", GetMouseDebug3());
	sprintf(&aaa4[0], "�}�E�X�̏��:%d", GetMouseDebug4());
	sprintf(&aaa5[0], "�}�E�X�̏��:%d", GetMouseDebug5());
	sprintf(&aaa6[0], "�}�E�X�̏��:%d", GetMouseDebug6());
	sprintf(&aaa7[0], "�}�E�X�̏��:%d", GetMouseDebug7());
	sprintf(&effect[0], "�G�t�F�N�g�̐�( %d )", GetEffectNum());

		switch (GetCameraMode())
		{
		case CAMERA_MODE_1:
			sprintf(&aCameraMode[0], "R�X�e�B�b�N�������� or [F3] ( �J�������[�h : ��l�̎��_ )");
			break;
		case CAMERA_MODE_3:
			sprintf(&aCameraMode[0], "R�X�e�B�b�N�������� or [F3] ( �J�������[�h : �O�l�̎��_ )");
			break;
		}

	//�e�L�X�g�̕`��
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
//���[�h�̐ݒ�
//====================================================================
void SetMode(MODE mode)
{
	//���݂̉��(���[�h)�̏I������
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

	g_mode = mode;	//���[�h��ݒ肷��

					//�V�������(���[�h)�̏���������
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
//���[�h������
//====================================================================
MODE GetMode(void)
{
	return g_mode;
}