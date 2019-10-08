#include <windows.h>
#include <crtdbg.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"EX01 : Hinh Chu Nhat Chay Quanh Man Hinh"

#define BOX_TEXTURE_PATH L"Resouce//box.png"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400

#define MAX_FRAME_RATE 10

#define BOX_WIDTH 40
#define BOX_HEIGHT 40

LPDIRECT3D9 d3d = NULL;						// Direct3D handle
LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

LPDIRECT3DSURFACE9 backBuffer = NULL;
LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

// texture object to store brick image
LPDIRECT3DTEXTURE9 texBox;
RECT rectWindow;

int flag = 1;
float box_x = 0;
float box_y = 0;

// 1- left
// 2- bottom
// 3- right 
// 4 - top

//            top
//     ||- - <= x - - ||
//	   ||			  A|
//left || y		     y||  right
//     |v			  ||
//     ||- - X => - - ||
//          bottom

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
void InitDirectX(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	GetClientRect(hWnd, &rectWindow);	// retrieve Window width & height 

	d3dpp.BackBufferHeight = rectWindow.bottom + 1;
	d3dpp.BackBufferWidth = rectWindow.right + 1;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);

	OutputDebugString(L"[INFO] InitGame is done\n");

	_RPT1(0, "[INFO] RECT botom  %d right %d \n", rectWindow.bottom, rectWindow.right);

	box_x = rectWindow.right - 40;
}

/*
	Load all game resources. In this example, only load brick image
*/
void LoadResources()
{
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(BOX_TEXTURE_PATH, &info);
	if (result != D3D_OK)
	{
		_RPT0(0, "[ERROR] GetImageInfoFromFile failed: %s\n", BOX_TEXTURE_PATH);
		return;
	}

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		BOX_TEXTURE_PATH,					// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 255, 255),			// Transparent color
		&info,
		NULL,
		&texBox);								// Created texture pointer

	if (result != D3D_OK)
	{
		_RPT0(0, "[ERROR] CreateTextureFromFile failed\n");
		return;
	}

	// init pos
	switch (flag)
	{
		// left
	case 1:
		box_x = 0;
		box_y = 0;
		break;
		// bottom
	case  2:
		box_x = 0;
		box_y = rectWindow.bottom - BOX_HEIGHT;
		break;
		// rigth
	case  3:
		box_x = rectWindow.right - BOX_WIDTH;
		box_y = rectWindow.bottom - BOX_HEIGHT;
		break;
		//top
	case  4:
		box_x = 0;
		box_y = rectWindow.right - BOX_WIDTH;
		break;

	default:
		break;
	}


	_RPT1(0, "[INFO] Texture loaded Ok: %s \n", BOX_TEXTURE_PATH);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{

	_RPT1(0, "[INFO] Box (%d, %d) \n", box_x, box_y);

	// Kiem tra co cham dich de chuyen flag
	if (box_y == rectWindow.bottom - BOX_HEIGHT && flag == 1)
	{
		flag = 2; // bottom
	}
	else if (box_x == rectWindow.right - BOX_WIDTH && flag == 2)
	{
		flag = 3;
	}
	else if (box_y == 0 && flag == 3)
	{
		flag = 4;
	}
	else if (box_x == 0 && flag == 4)
	{
		flag = 1;
	}

	switch (flag)
	{
	case 1:
		if (box_y < rectWindow.bottom - BOX_HEIGHT)
		{
			box_y += 5;
		}
		else
		{
			box_y = rectWindow.bottom - BOX_HEIGHT;
		}
		break;
	case 2:
		if (box_x < rectWindow.right - BOX_WIDTH)
		{
			box_x += 5;
		}
		else
		{
			box_x = rectWindow.right - BOX_WIDTH;
		}
		break;
	case 3:
		if (box_y >= 0)
		{
			box_y -= 5;
		}
		else
		{
			box_y = 0;
		}
		break;
	case 4:
		if (box_x >= 0)
		{
			box_x -= 5;
		}
		else
		{
			box_x = 0;
		}
		break;
	default:
		break;
	}
}

/*
Render a frame
*/
void Render()
{
	if (d3ddv->BeginScene())
	{
		// Clear screen (back buffer) with a color
		d3ddv->ColorFill(backBuffer, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		// Render

		RECT r;
		r.top = 0;
		r.bottom = BOX_HEIGHT;
		r.left = 0;
		r.right = BOX_WIDTH;

		spriteHandler->Draw(texBox, &r, NULL, &D3DXVECTOR3(box_x, box_y, 0), D3DCOLOR_XRGB(255, 255, 255));

		// End
		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE,// WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);
	InitDirectX(hWnd);


	LoadResources();
	Run();

	return 0;
}