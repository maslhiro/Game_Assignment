#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Game.h"
#include "GameObject.h"
#include "Ball.h"
#include "Bar.h"
#include "HubPoint.h"

#define WINDOW_CLASS_NAME L"EX02"
#define MAIN_WINDOW_TITLE L"EX02: Game danh bong"

#define BALL_TEXTURE_PATH L"ball.png"
#define MARIO_TEXTURE_PATH L"mario.png"
#define BAR_TEXTURE_PATH L"bar.png"
#define N0_TEXTURE_PATH L"T0.png"
#define N1_TEXTURE_PATH L"T1.png"
#define N2_TEXTURE_PATH L"T2.png"
#define N3_TEXTURE_PATH L"T3.png"
#define N4_TEXTURE_PATH L"T4.png"
#define N5_TEXTURE_PATH L"T5.png"
#define N6_TEXTURE_PATH L"T6.png"
#define N7_TEXTURE_PATH L"T7.png"
#define N8_TEXTURE_PATH L"T8.png"
#define N9_TEXTURE_PATH L"T9.png"
#define N99_TEXTURE_PATH L"T99.png"


#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

#define MAX_FRAME_RATE 90

CGame *game;
CMario *mario;
CBall *ball;
CBar *bar1, *bar2;
HubPoint *_hub;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		CGame::GetInstance()->keyDown(wParam);
		break;
	case WM_KEYUP:
		CGame::GetInstance()->keyUp(wParam);
		break;
	case WM_MOUSEMOVE:
		CGame::GetInstance()->mouseMove(GET_Y_LPARAM(lParam));
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources. In this example, create a brick object and mario object
*/
void LoadResources()
{
	//mario = new CMario(MARIO_TEXTURE_PATH);
	//mario->SetPosition(10.0f, 130.0f);

	ball = new CBall(BALL_TEXTURE_PATH);

	bar1 = new CBar(BAR_TEXTURE_PATH);
	bar1->setId(1);
	bar1->SetPosition(0, 0);
	bar1->setOrigin(0, 0);
	bar1->setVy(2.0f);

	bar2 = new CBar(BAR_TEXTURE_PATH);
	bar2->setId(2);
	bar2->SetPosition(CGame::GetInstance()->getWidth(), 0);
	bar2->setOrigin(bar2->getWidth(), 0);
	bar2->setVy(2.0f);

	_hub = new HubPoint();
	_hub->add(eTexture::TEX_T, N99_TEXTURE_PATH);
	_hub->add(eTexture::TEX_0, N0_TEXTURE_PATH);
	_hub->add(eTexture::TEX_1, N1_TEXTURE_PATH);
	_hub->add(eTexture::TEX_2, N2_TEXTURE_PATH);
	_hub->add(eTexture::TEX_3, N3_TEXTURE_PATH);
	_hub->add(eTexture::TEX_4, N4_TEXTURE_PATH);
	_hub->add(eTexture::TEX_5, N5_TEXTURE_PATH);
	_hub->add(eTexture::TEX_6, N6_TEXTURE_PATH);
	_hub->add(eTexture::TEX_7, N7_TEXTURE_PATH);
	_hub->add(eTexture::TEX_8, N8_TEXTURE_PATH);
	_hub->add(eTexture::TEX_9, N9_TEXTURE_PATH);
	_hub->setPos(game->getWidth() / 2, 40);

}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{

	bar1->Update(dt);
	bar2->Update(dt);

	ball->update(dt, bar1->getBoundingBox(), bar2->getBoundingBox());

	//_hub->update(dt);
}

/*
	Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		ball->Render();
		bar1->Render();
		bar2->Render();
		//_hub->render();

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
			WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
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

	game = CGame::GetInstance();
	game->Init(hWnd);

	LoadResources();
	Run();

	return 0;
}