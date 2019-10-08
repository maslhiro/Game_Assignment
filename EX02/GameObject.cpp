#include <d3dx9.h>

#include "Game.h"
#include "GameObject.h"

RECT CGameObject::getBoundingBox()
{
	RECT _rect;

	_rect.top = y - _height / 2;
	_rect.bottom = y + _height / 2;

	_rect.left = x - _width / 2;
	_rect.right = x + _width / 2;

	return _rect;
}

CGameObject::CGameObject()
{

}

CGameObject::CGameObject(LPCWSTR texturePath)
{
	x = y = 0;

	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(texturePath, &info);
	if (result != D3D_OK)
	{
		_RPT1(0, "[ERROR] GetImageInfoFromFile failed: %s\n", texturePath);
		return;
	}

	_height = info.Height;
	_width = info.Width;

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		texturePath,						// Path to the image to load
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
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}
	_origin = D3DXVECTOR3((float)info.Width / 2, (float)info.Height / 2, 0);

	_RPT1(0, "[INFO] Texture loaded Ok: %s \n", texturePath);
}

bool CGameObject::checkCollision(RECT r)
{
	RECT _cur = getBoundingBox();
	return !(
		_cur.left > r.right ||
		_cur.right < r.left ||
		_cur.top > r.bottom ||
		_cur.bottom < r.top);
}

void CGameObject::Update(DWORD dt)
{
}

void CGameObject::Render()
{
	CGame::GetInstance()->Draw(x, y, _origin, texture);
}


CGameObject::~CGameObject()
{
	if (texture != NULL) texture->Release();
}

void CMario::Update(DWORD dt)
{
	x += 0.1f*dt;
	if (x > 320) x = 0;
}
