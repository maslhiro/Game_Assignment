#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <crtdbg.h>

class CGame
{
	static CGame * __instance;
	HWND hWnd;									// Window handle

	RECT _rectWindow;

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	int _keyCode;

	int _posMouse;
	int _prePosMouse = 250;

public:
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture);
	void Draw(float, float, D3DXVECTOR3, LPDIRECT3DTEXTURE9);

	void keyDown(int);
	void keyUp(int);

	void mouseMove(int);
	void mouseHover(int);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	static CGame * GetInstance();

	float getWidth() { return (float)_rectWindow.right; };
	float getHeight() { return (float)_rectWindow.bottom; };

	int getKeyDown() { return _keyCode; }

	// duong => keo xuong 
	// am => keo len
	int distancePosMouse() { return _posMouse - _prePosMouse; }

	~CGame();
};